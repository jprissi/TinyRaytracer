#include "raytracer.h"
#include <stdio.h>
#include <vector>
#include "ray.hpp"
#include <cassert>

const int AA_samples = 16;     // Antialiasing samples
const float scale = 1.f / 256; // depends on image resolution // TODO
const int max_hit_bounces = 3; // Maximum number of ray hits

const vect3 X{1 * scale, 0, 0};
// const vect3 X{0, 0, 0};
const vect3 Y{0, 1 * scale, 0};
const vect3 Z{0, 0, 1};

// Initialize random seed & generator
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> rand_gen(-0.5f, 0.5f);

Raytracer::Raytracer(Scene *p_scene) //: //, camera(camera)
{
  this->scene = *p_scene;
  scene.init();
};

void Raytracer::compute(std::vector<unsigned char> &img, Camera const &camera)
{
  img.reserve(4 * camera.getWidth() * camera.getHeight());
  vect3 outgoing_ray_origin;
  vect3 outgoing_ray_dir;

  // Left-handed base with z forward
  for (int y = camera.getHeight() / 2; y >= -camera.getHeight() / 2 + 1; y--)
  {
    for (int x = -camera.getWidth() / 2 + 1; x <= camera.getWidth() / 2; x++)
    {

      Color final_color = computePixel(x, y, outgoing_ray_origin, outgoing_ray_dir, camera);
      export_color(final_color, img);
    }
  }

  // Exporting file to .png using lodepng library
  const char filename[] = "img.png";
  unsigned error =
      lodepng::encode(filename, img, camera.getWidth(), camera.getHeight());

  if (error)
  {
    std::cout << "encoder error" << error << ": " << lodepng_error_text(error)
              << std::endl;
  }
  else
  {
    std::cout << "Exported img.png!\n";
  }
};
Color Raytracer::computePixel(int const &x, int const &y, vect3 &outgoing_ray_origin, vect3 &outgoing_ray_dir, Camera const &camera)
{
  Color final_color = {0, 0, 0};
  // We use pixel centers (x-0.5), (y-0.5)

  matrix X_matrix(3, 1, {0, 0, 0});
  X_matrix(0, 0) = X.x;
  matrix X_temp = camera.getOrientation() * X_matrix; // X_temp (3, 1)
  vect3 X(X_temp(0, 0), X_temp(1, 0), X_temp(2, 0));
  // std::cout << X << std::endl;
  matrix Y_matrix(3, 1, {0, 0, 0});
  Y_matrix(1, 0) = Y.y;
  matrix Y_temp = camera.getOrientation() * Y_matrix; // X_temp (3, 1)
  vect3 Y(Y_temp(0, 0), Y_temp(1, 0), Y_temp(2, 0));
  // std::cout << Y << std::endl;
  matrix Z_matrix(3, 1, {0, 0, 1});
  matrix Z_temp = camera.getOrientation() * Z_matrix; // X_temp (3, 1)
  vect3 Z(Z_temp(0, 0), Z_temp(1, 0), Z_temp(2, 0));
  // std::cout << Z << std::endl;
  // std::cout << Z_temp << std::endl;
  // std::cout << Z_temp(0, 0) << "; " << Z_temp(1, 0) << "; " << Z_temp(2, 0) << std::endl;
  // int AA_samples = 32;
  for (int i = 0; i < AA_samples; i++)
  {
    float x_offset = rand_gen(gen);
    float y_offset = rand_gen(gen);

    ray.direction =
        !vect3{X * (x - 0.5 + x_offset) + Y * (y - 0.5 + y_offset) +
               Z};
    ray.origin = camera.getPosition() + vect3{0, 1, -4};

    float lambda = 0.9f; // Randomize energy
    ray.energy = (1 - lambda) * (random_offset() + 0.5) + lambda;
    ray.energy = 1.5f;

    // adding depth of field - messes up the background?
    vect3 sensor_shift{random_offset() * camera.aperture,
                       random_offset() * camera.aperture, 0.0f};
    // vect3 sensor_shift;

    ray.origin = ray.origin + sensor_shift;
    ray.direction =
        !(ray.direction - sensor_shift * (1. / camera.focalDistance));

    Color color;
    for (int n_bounces = 0; n_bounces < max_hit_bounces; n_bounces++)
    {
      if (!propagate_ray(&outgoing_ray_origin, &outgoing_ray_dir, &color))
      {
        break; // If no bounce
      };
    }

    final_color += color * (1.0f / AA_samples);
  }
  return final_color;
};

bool Raytracer::propagate_ray(vect3 *p_outgoing_ray_origin, vect3 *p_outgoing_ray_dir,
                              Color *p_color)
{
  float distance_to_hit;
  bool object_hit{false};
  float closest_obj_dist{std::numeric_limits<float>::max()};
  Object *closest_object_ptr{nullptr};
  Material hit_material{};
  vect3 hit_color{0, 0, 0};

  for (const auto &object : scene.scene_objects)
  {
    if (object->is_hit(ray.origin, ray.direction, *p_outgoing_ray_origin,
                       *p_outgoing_ray_dir, distance_to_hit, hit_material,
                       hit_color, ray.energy, scene.lightSource))
    {
      // assert(false); // No object hit
      object_hit = true;
      if (distance_to_hit < closest_obj_dist)
      {
        closest_obj_dist = distance_to_hit;
        closest_object_ptr = object;
      }
    }
  }
  if (object_hit and closest_object_ptr)
  {
    closest_object_ptr->is_hit(ray.origin, ray.direction,
                               *p_outgoing_ray_origin, *p_outgoing_ray_dir,
                               distance_to_hit, hit_material, hit_color,
                               ray.energy, scene.lightSource);
    ray.origin = *p_outgoing_ray_origin;
    ray.direction = *p_outgoing_ray_dir;

    *p_color += hit_color;
    ray.energy *= hit_material.reflectivity;
    return true;
  }
  else
  {
    // No object in reach (case 1 : we hit the ground, case 2 : we "hit" the
    // sky)
    if (ray.direction * Y < 0)
    {
      hit_color = ground_color(ray.direction, ray.origin);
    }
    else
    {
      hit_color = sky_color(ray.direction);
    }
    *p_color =
        *p_color + hit_color * ray.energy; // No object hit, draw sky or ground
    return false;
  }
}
float Raytracer::random_offset()
{
  // Random value uniform in [-0.5, 0.5]
  return static_cast<float>(rand()) / RAND_MAX - 0.5f;
}

void Raytracer::export_color(vect3 color, std::vector<unsigned char> &img)
{
  /* convert color vector to RVBA channels */
  img.push_back(clip(color.x));
  img.push_back(clip(color.y));
  img.push_back(clip(color.z));
  img.push_back(255);
}

vect3 Raytracer::sky_color(vect3 direction)
{
  /* Computes the color of the sky based on ray direction
   * Input : vect3 direction, the direction of the ray
   * Output : vect3, the color of the sky in this direction
   */
  return vect3{0.3, 0.3, 0.6} * 255.f * std::pow(1 - direction.y, 2);
}

vect3 Raytracer::ground_color(vect3 direction, vect3 origin)
{
  vect3 hit_color;

  float dist = -origin.y / direction.y;
  float x = origin.x + dist * direction.x;
  float y = 0; // Ground is at 0-level
  float z = origin.z + dist * direction.z;

  vect3 hit_point = vect3{x, y, z};
  // float distance_to_hit;
  bool is_directly_lit{true};

  // Handle shadowing
  vect3 dir = !(scene.lightSource.pos - hit_point);
  for (const auto &object : scene.scene_objects)
  {
    if (object->is_hit(hit_point, dir))
    {
      is_directly_lit = false;
      break;
    }
  }

  if (scene.ground_texture != nullptr)
  {
    float scale = 200.0f;
    hit_color = scene.ground_texture->getColor(vect2{hit_point.z, hit_point.x} * (scale), vect3{0., 0., 0.});
    return hit_color;
  }
  // White

  hit_color = vect3{1.0, 1.0, 1.0} * 255.f;
  if ((int)std::abs(std::floor(x)) % 2 ==
      (int)std::abs(std::floor(z)) % 2)
  {
    hit_color = vect3{0.0, 0.0, 0.0} * 255.f; // Black if both x and y are odd/even
  }

  if (!is_directly_lit)
  {
    return hit_color * scene.lightSource.k_ambient;
  }
  return hit_color;
}

float Raytracer::clip(float component)
{
  int value = (int)std::max(0.f, std::min(255.f, component));
  return value;
}
