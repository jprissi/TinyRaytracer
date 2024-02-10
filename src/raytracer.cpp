#include "raytracer.h"
#include <stdio.h>
#include <vector>
#include "ray.hpp"

const int AA_samples = 32;     // Antialiasing samples
const float scale = 1.f / 512; // depends on image resolution
const int max_hit_bounces = 3; // Maximum number of ray hits

const vect3 X{1 * scale, 0, 0};
const vect3 Y{0, 1 * scale, 0};
const vect3 Z{0, 0, 1};

// Initialize random seed & generator
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> rand_gen(-0.5f, 0.5f);

Raytracer::Raytracer(Scene &scene, Camera const &camera) : camera(camera)
{
  scene.init();
};

void Raytracer::compute(std::vector<unsigned char> &img)
{
  img.reserve(4 * camera.getWidth() * camera.getHeight());

  vect3 outgoing_ray_origin;
  vect3 outgoing_ray_dir;
  Color final_color;

  // Left-handed base with z forward
  for (int y = 256; y >= -255; y--)
  {
    for (int x = -255; x <= 256; x++)
    {
      // We use pixel centers (x-0.5), (y-0.5)
      final_color = {0, 0, 0};
      for (int i = 0; i < AA_samples; i++)
      {
        float x_offset = rand_gen(gen);
        float y_offset = rand_gen(gen);

        ray.direction =
            !vect3{X * (x - 0.5 + x_offset) + Y * (y - 0.5 + y_offset) +
                   Z};
        ray.origin = vect3{0, 1, -4};
        ray.energy =
            1.0f; // 0.2*random_offset()+0.8f;//1.0f;//random_offset(); //1.0f;

        // adding depth of field
        vect3 sensor_shift{random_offset() * camera.aperture,
                           random_offset() * camera.aperture, 0.0f};
        ray.origin = ray.origin + sensor_shift;
        ray.direction =
            !(ray.direction - sensor_shift * (1. / camera.focalDistance));

        Color color;
        for (int number_bounces = 0; number_bounces < max_hit_bounces;
             number_bounces++)
        {
          if (!propagate_ray(&outgoing_ray_origin, &outgoing_ray_dir, &color))
          {
            // If no bounce
            break;
          };
        }
        final_color = final_color + color * (1.0f / AA_samples); // TODO: add += operator to vect3
      }

      // propagate_ray(&outgoing_ray_origin, &outgoing_ray_dir, scene_objects);
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

bool Raytracer::propagate_ray(vect3 *p_outgoing_ray_origin, vect3 *p_outgoing_ray_dir,
                              vect3 *p_color)
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

    *p_color = *p_color + hit_color;
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

  float dist = -origin.y / direction.y;
  float x = origin.x + dist * direction.x;
  float y = 0; // Ground is at 0-level
  float z = origin.z + dist * direction.z;

  vect3 hit_point = vect3{x, y, z};
  // float distance_to_hit;
  bool is_directly_lit{true};

  // Material hit_material{};
  vect3 hit_color{0, 0, 0};

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

  if ((int)std::abs(std::floor(x)) % 2 ==
      (int)std::abs(std::floor(z)) % 2)
  { // Black if both x and y are odd/even
    hit_color = vect3{0.0, 0.0, 0.0} * 255.f;
  }
  else
  {
    hit_color = vect3{1.0, 1.0, 1.0} * 255.f;
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
