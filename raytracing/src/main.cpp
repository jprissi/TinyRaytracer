#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <vector>

#include "vector.h"
#include "camera.cpp"
#include "object.cpp"

using namespace std;



const float scale = 1.f/512; //depends on image resolution
// int max_hit_bounces = 3; //Maximum number of ray hits
int max_hit_bounces = 1000; //We're doing raytracing and not raycasting so it's fine

const Vect X{1*scale, 0, 0};
const Vect Y{0, 1*scale, 0};
const Vect Z{0, 0, 1};

Camera cam{512, 512};
  
Vect ray_origin{0,1,-4};
Vect ray_direction{0, 0, 0};
float ray_energy = 1.0f;
Vect color{0,0,0};
Vect final_color{0,0,0};
Material hit_material{};

Vect sky_color(Vect direction) {
  return Vect{1.0, 1.0, 1.0}*255.f * std::pow(1-direction.getY(), 2);
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
    scene_objects.back()->set_reflectivity(0.9f);
    // scene_objects.push_back(new Triangle({-0.1,0,0},
    //                                      {0.1,0,0},
    //                                      {0,0.1,0}));
    // scene_objects.back()->set_color({255, 0, 0});

    scene_objects.push_back(new Triangle({2,0,-5},
                                         {-2,0,-5},
                                         {0,3,-4.9}));
    scene_objects.back()->set_color({0,255,0});
    scene_objects.back()->set_reflectivity(0.5f);
}
void export_color(Vect color, ofstream* p_outfile) {
  *p_outfile
    <<  (int)(std::max(0.f, std::min(255.f, color.x))+0.5)
    << " "
    <<  (int)(std::max(0.f, std::min(255.f, color.y))+0.5)
    << " "
    << (int)(std::max(0.f, std::min(255.f, color.z))+0.5)
    << " ";
}
bool propagate_ray(Vect* p_outgoing_ray_origin, Vect* p_outgoing_ray_dir, std::vector<Object*> scene_objects){
  float distance_to_hit;
  bool object_hit{false};
  float closest_obj_dist{std::numeric_limits<float>::max()};
  Object* closest_object_ptr{nullptr};

  for (const auto& object : scene_objects) {
    if(object->is_hit(ray_origin, ray_direction, *p_outgoing_ray_origin, *p_outgoing_ray_dir, distance_to_hit, hit_material)){
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
      hit_material
    );
    ray_origin = *p_outgoing_ray_origin;
    ray_direction = *p_outgoing_ray_dir;

    
    final_color = final_color + hit_material.color*(1-hit_material.reflectivity)*ray_energy;
    ray_energy *= hit_material.reflectivity;
    return true;
  } else {
    final_color = final_color + ray(ray_direction, ray_origin)*ray_energy; //No object hit, draw sky or ground
    // break;
    return false;
  }
}
int main() {
  
  Vect outgoing_ray_origin;
  Vect outgoing_ray_dir;

  std::vector<Object*> scene_objects;

 //TODO: Transparency (e.g. glass of water)
 
  init_scene(scene_objects);
    
  // Creating a ppm image file
  ofstream outfile("img.ppm");
  outfile << "P3 512 512 255 ";

  //Left-handed base with z forward
  for (int y=256; y>=-255; y--){
    for (int x=-255; x<= 256; x++){
      //For each image pixel...
      //We use pixel centers (x-0.5), (y-0.5)
      ray_direction = !Vect{X*(x-0.5) + Y*(y-0.5) + Z}; //!(Vect{(x-0.5), y-0.5, 0} - ray_origin);
      ray_origin = Vect{0,1,-4};
      ray_energy = 1.0f;
      final_color = {0,0,0};
      for(int number_bounces=0; number_bounces < max_hit_bounces; number_bounces++) {
          if(!propagate_ray(&outgoing_ray_origin, &outgoing_ray_dir, scene_objects)){
            //If no bounce
            break;
          };
      }
      export_color(final_color, &outfile);
    }
  }

  cout << "Exported img.ppm!\n";  
  return 0;
}


