#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <vector>
//#include "vector.h"
#include "vector.h"
#include "camera.cpp"
#include "object.cpp"

using namespace std;

  float scale = 1.f/512; //depends on image resolution

  const Vect X{1.0*scale, 0, 0};
  const Vect Y{0, 1.0*scale, 0};
  const Vect Z{0, 0, 1};
  

Vect sky_color(Vect direction) {
  //cout << direction.getY() << "\n";
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
    //return Vect{0, 0, 0};
  }
  if(direction.y>=0){
    //"hitting" sky
    return sky_color(direction);
  }
}

int main() { 

  Camera cam{512, 512};
  
  Vect ray_origin{0,1,-4};
  Vect ray_direction{0, 0, 0};

  Vect color{0,0,0};

  std::vector<Object*> scene_objects;

  scene_objects.push_back(new Triangle{{-1,0,0}, {1,0,0}, {0,1.73,0}});
  scene_objects.back()->set_color({0,0,255});

  scene_objects.push_back(new Triangle{{2,0,2}, {1,1.73,2}, {0,0,2}});
  scene_objects.back()->set_color({0,255,0});

  scene_objects.push_back(new Triangle{{-0.25,0.75,-1}, {0.75,0.75,-1}, {0.25,2,-1}});
  scene_objects.back()->set_color({0,255,255});
  // float v = ray_origin*Z;
  //cout << "origin*Z = " << v;

  // Testing dot product
  // cout << "origin(" << ray_origin*X << ", " << ray_origin*Y << ", " << ray_origin*Z << ")\n";


  
  // Creating a ppm image file
  ofstream outfile("img.ppm");
  outfile << "P3 512 512 255 ";

  //Output vect
  //cout << color;
  //Test colors:
  // cout << (Vect{1.0, 1.0, 1.0}*255.f * std::pow(1-Vect{-1.0, 0.0, 0.0}*Y, 2)).getX() << "\n";
  // cout << (Vect{1.0, 1.0, 1.0}*255.f * std::pow(1-Vect{-1.0, 1.0, 0.0}*Y, 2)).getX() << "\n";
  // cout << (Vect{1.0, 1.0, 1.0}*255.f * std::pow(1-Vect{0.0, 0.0, 0.0}*Y, 2)).getX() << "\n";
  // cout << (Vect{1.0, 1.0, 1.0}*255.f * std::pow(1-Vect{0.0, 1.0, 0.0}*Y, 2)).getX() << "\n";
  
  //Left-handed base with z forward
  //cout << "\n";
  // cout << !(Vect{-255.5, 0.5, 0} - ray_origin).y << "\n";
  // cout << !(Vect{0.5, 0.5, 0} - ray_origin).y << "\n";
  // cout << !(Vect{-255.5, -125, 0} - ray_origin).y << "\n";
  // cout << (Vect{1,1,0})*(1.f/((Vect{1,1,0}).length())).x << "\n";
  //   cout << (Vect{1,1,0})*(1.f/((Vect{1,1,0}).length())).y << "\n";
  //     cout << (Vect{1,1,0})*(1.f/((Vect{1,1,0}).length())).z << "\n";
  // cout << !(Vect{1,1,0}).length() << "\n";
  //  cout << !(Vect{1,1,0}).x << "\n";
  for (int y=256; y>=-255; y--){
    //cout << "\n";
    for (int x=-255; x<= 256; x++){
      //if(x%32==0){cout << "(" << x << "," << y << ")";}
      //For each image pixel...
      //We use pixel centers (x-0.5), (y-0.5)
      // cout << " " << ray(x,y);
      ray_direction = !Vect{X*(x-0.5) + Y*(y-0.5) + Z}; //!(Vect{(x-0.5), y-0.5, 0} - ray_origin);
      //if (x%32==0) {cout << ray_direction.y << " ";}
      //if (x%32==0) {cout << (!Vect{X*(x-0.5) + Y*(y-0.5) + Z}).y << " ";}
      //cout << "dir(" << ray_direction*X << ", " << ray_direction*Y << ", " << ray_direction*Z << ")\n";

      float distance_to_hit;
      bool object_hit{false};
      float min_hit_dist{std::numeric_limits<float>::max()};
      Object* closest_object_ptr{nullptr};

      for (const auto& object : scene_objects) {
        if(object->is_hit(ray_origin, ray_direction, distance_to_hit, color)){
          object_hit = true;
          if (distance_to_hit < min_hit_dist) {
            min_hit_dist = distance_to_hit;
            closest_object_ptr = object;
          }
        }
      }
      // if(object_hit) cout<<"hit!\n";
      
      if(object_hit and closest_object_ptr) {
        closest_object_ptr->is_hit(ray_origin, ray_direction, distance_to_hit, color);
      } else {
        color = ray(ray_direction, ray_origin); //No object hit, draw sky or ground
      }
      
      //cout << "color(R=" << color*X << ", V=" << color*Y << ", B=" << color*Z << ")\n";
      outfile
        <<  (int)(std::max(0.f, std::min(255.f, color.x))+0.5)
        << " "
        <<  (int)(std::max(0.f, std::min(255.f, color.y))+0.5)
        << " "
        << (int)(std::max(0.f, std::min(255.f, color.z))+0.5) //static_cast<unsigned char>()
        << " ";
    }
    // cout << "\n";
  }

  cout << "Exported img.ppm!\n";  
  return 0;
}


