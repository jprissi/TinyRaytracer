/**
 * 
 * This project was written while following the slides for the "Let's build a simple raytracer" course available on Github
 * I chose to slightly change the architecture. This is a first time writing C++.
 * 
 * Bonus interesting features to add as exercices :
 * 
 * - Transparency (make a glass of water with an object inside)
 * 
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <vector>
#include <random>

#include "vector.h"
#include "light.h"
#include "lodepng.h"

//TEMPORARY
#include "camera.cpp"
#include "object.cpp"

using namespace std;

//Initialize random seed & generator
random_device rd;
mt19937 gen(rd());
uniform_real_distribution<float> rand_gen(-0.5f, 0.5f);

// INITIALIZATION STUFF / CONSTANTS
const int AA_samples = 32; //Antialiasing samples
const float scale = 1.f/512; //depends on image resolution
// int max_hit_bounces = 3; //Maximum number of ray hits
int max_hit_bounces = 1000; //We're not doing realtime computation so it is fine


const Vect X{1*scale, 0, 0};
const Vect Y{0, 1*scale, 0};
const Vect Z{0, 0, 1};

Camera cam{512, 512};
vector<unsigned char> img;

Vect ray_origin{0,1,-4};
Vect ray_direction{0, 0, 0};
float ray_energy = 1.0f;
Vect color{0,0,0};
Vect final_color{0,0,0};
Vect v{2,3,-4};
Light light_src{v};

Vect sky_color(Vect direction) {
  return Vect{0.3, 0.3, 0.6}*255.f * std::pow(1-direction.getY(), 2);
}

Vect ground_color(Vect direction, Vect origin){
  float dist = - origin.y / direction.y;
  float x = origin.x + dist*direction.x;
  float z = origin.z + dist*direction.z;

  if (
    (int)std::abs(std::floor(x)) % 2 ==
    (int)std::abs(std::floor(z)) % 2 ) { //Black if both x and y are odd/even
   return Vect{0.0,0.0,0.0}*255.f;
  } 
  return Vect{1.0,1.0,1.0}*255.f;
}

Vect ray(Vect direction, Vect origin) {
  if (direction*Y<0) {
    //hitting ground
    return ground_color(direction, origin);
  }
    //"hitting" sky
    return sky_color(direction);
}

void init_scene(std::vector<Object*>& scene_objects){
    scene_objects.push_back(new Triangle({-2,0.5f,-1},
                                         {2,0,-1},
                                         {0,3,-1.1}));
    scene_objects.back()->set_color({0,0,255});
    scene_objects.back()->set_reflectivity(.8f);

    scene_objects.push_back(new Sphere({0, 1, -2}, .3f));
    scene_objects.back()->set_color({0,255,0});
    scene_objects.back()->set_reflectivity(0.1f);

    scene_objects.push_back(new Sphere({-1, 1.5f, -2}, .5f));
    scene_objects.back()->set_color({255,0,0});
    scene_objects.back()->set_reflectivity(.7f);

    // Vect light_pos = {2,3,2};
    // light_src = Light{light_pos};
    // light_src.pos = Vect{2, 3, 2};
    // scene_objects.push_back(new Triangle({-0.1,0,0},
    //                                      {0.1,0,0},
    //                                      {0,0.1,0}));
    // scene_objects.back()->set_color({255, 0, 0});

    // scene_objects.push_back(new Triangle({2,0,-5},
    //                                      {-2,0,-5},
    //                                      {0,3,-4.9}));
    // scene_objects.back()->set_color({0,255,0});
    // scene_objects.back()->set_reflectivity(0.5f);
}
void export_color(Vect color) {
  img.push_back(
    (unsigned char)(std::max(0.f, std::min(255.f, color.x))+0.5)
  );
  img.push_back(
    (std::max(0.f, std::min(255.f, color.y))+0.5)
  );
  img.push_back(
    (std::max(0.f, std::min(255.f, color.z))+0.5)
  );
  img.push_back(
    255
  );
}
bool propagate_ray(Vect* p_outgoing_ray_origin, Vect* p_outgoing_ray_dir, std::vector<Object*> scene_objects, Vect* p_color){
  float distance_to_hit;
  bool object_hit{false};
  float closest_obj_dist{std::numeric_limits<float>::max()};
  Object* closest_object_ptr{nullptr};
  Material hit_material{};
  Vect hit_color{0,0,0};
  for (const auto& object : scene_objects) {
    if(object->is_hit(ray_origin, ray_direction, *p_outgoing_ray_origin, *p_outgoing_ray_dir, distance_to_hit, hit_material, hit_color, ray_energy, light_src)){
      object_hit = true;
      if (distance_to_hit < closest_obj_dist) {
        closest_obj_dist = distance_to_hit;
        closest_object_ptr = object;
      }
    }
  }
  if(object_hit and closest_object_ptr) {
    closest_object_ptr->is_hit(
      ray_origin, 
      ray_direction, 
      *p_outgoing_ray_origin, 
      *p_outgoing_ray_dir, 
      distance_to_hit, 
      hit_material,
      hit_color,
      ray_energy,
      light_src
    );
    ray_origin = *p_outgoing_ray_origin;
    ray_direction = *p_outgoing_ray_dir;

    
    *p_color = *p_color + hit_color;
    ray_energy *= hit_material.reflectivity;
    return true;
  } else {
    *p_color = *p_color + ray(ray_direction, ray_origin)*ray_energy; //No object hit, draw sky or ground
    // break;
    return false;
  }
}
int main() {
  Vect outgoing_ray_origin;
  Vect outgoing_ray_dir;

  std::vector<Object*> scene_objects;
 
  init_scene(scene_objects);

  img.reserve(4*cam.getWidth()*cam.getHeight());

  //Left-handed base with z forward
  for (int y=256; y>=-255; y--){
    for (int x=-255; x<= 256; x++){
      //For each image pixel...
      //We use pixel centers (x-0.5), (y-0.5)
      final_color = {0,0,0};
      for(int i=0; i<AA_samples; i++){
        color = {0,0,0};
        float x_offset = rand_gen(gen);
        float y_offset = rand_gen(gen);

        ray_direction = !Vect{X*(x-0.5+x_offset) + Y*(y-0.5+y_offset) + Z}; //!(Vect{(x-0.5), y-0.5, 0} - ray_origin);
        ray_origin = Vect{0,1,-4};
        ray_energy = 1.0f;
        
        for(int number_bounces=0; number_bounces < max_hit_bounces; number_bounces++) {
          if(!propagate_ray(&outgoing_ray_origin, &outgoing_ray_dir, scene_objects, &color)){
            //If no bounce
            break;
          };
        }
        final_color = final_color + color*(1.0f/AA_samples);
      }
      
      // propagate_ray(&outgoing_ray_origin, &outgoing_ray_dir, scene_objects);
      export_color(final_color);
    }
  }
  const char* filename = "img.png";
  unsigned error = lodepng::encode(filename, img, cam.getWidth(), cam.getHeight());

  if(error) {
    cout << "encoder error" << error << ": "<< lodepng_error_text(error) << endl;
  } else {
      cout << "Exported img.png!\n";  
  }

  return 0;
}


