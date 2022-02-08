/**
 * 
 * This project was written while following the slides for the "Let's build a simple raytracer" course available on Github
 * I chose to slightly change the architecture. This is a first time writing C++.
 * 
 * Bonus interesting features to add as exercices :
 * 
 * - Transparency (make a glass of water with an object inside)
 * 
 * Warning: We use a left-handed base with z forward (as it is apparently common to do in the computer graphics field)
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
int max_hit_bounces = 100; //We're not doing realtime computation so it is fine


const Vect X{1*scale, 0, 0};
const Vect Y{0, 1*scale, 0};
const Vect Z{0, 0, 1};

Camera cam{512, 512};
float focal_distance{2.5f};
float aperture{0.1f};

vector<unsigned char> img;

// Ray initial parameters (#TODO ray object)
Vect ray_origin{0,1,-4};
Vect ray_direction{0, 0, 0};
Vect color{0,0,0};
Vect final_color{0,0,0};
float ray_energy = 1.0f;


// Defining light source
Light light_src; //(uses default constructor)

// Container for the objects of the scene
std::vector<Object*> scene_objects;

//DEBUG Variables


Vect sky_color(Vect direction) {
  /* Computes the color of the sky based on ray direction
   * Input : Vect direction, the direction of the ray
   * Output : Vect, the color of the sky in this direction
   */
  return Vect{0.3, 0.3, 0.6} * 255.f
			     * std::pow(1-direction.getY(), 2);
}

Vect ground_color(Vect direction, Vect origin){
  
  float dist = - origin.y / direction.y;
  float x = origin.x + dist*direction.x;
  float y = 0; //Ground is at 0-level
  float z = origin.z + dist*direction.z;

  Vect hit_point = Vect{x, y, z};
  //float distance_to_hit;
  bool is_directly_lit{true};

  //Material hit_material{};
  Vect hit_color{0,0,0};

  //Handle shadowing
  Vect dir = !(light_src.pos-hit_point);
  //cout << dir <<endl;
  for (const auto& object : scene_objects) {
    //cout<<dir<<endl;
    if(object->is_hit(hit_point, dir)){
      //cout<<dir<<endl;
      is_directly_lit = false;
      break;
    }
    }

  if ((int)std::abs(std::floor(x)) % 2 == (int)std::abs(std::floor(z)) % 2 ) { //Black if both x and y are odd/even
    hit_color = Vect{0.0,0.0,0.0}*255.f;
  } else {
    hit_color = Vect{1.0,1.0,1.0}*255.f;
  }

  if(!is_directly_lit){
    return hit_color*light_src.k_ambient;
  }
  return hit_color;
}

void add_object_to_scene(Object* obj, Vect color, float reflectivity){
  //obj->set_pos(pos); //Not adapted to triangle object for now #TODO
  scene_objects.push_back(obj);
  scene_objects.back()->set_color(color);
  scene_objects.back()->set_reflectivity(reflectivity);
}

void init_scene(){
  /* 
   * Populate the scene with objects
   * Possible improvement : populate scene from .yaml file
   */

  //Octahedron (as in example from course)
  /// Octahedron (8 triangles)
  /// Bottom half
  scene_objects.push_back(new Triangle{{ 0, 0, 0},
                                       {-1, 1, 0},
                                       { 0, 1, 1}});
scene_objects.back()->set_color({0,0,255});
 scene_objects.back()->set_reflectivity(.8f);
  scene_objects.push_back(new Triangle{{ 0, 0, 0},
                                       { 0, 1,-1},
                                       {-1, 1, 0}});
scene_objects.back()->set_color({0,0,255});
 scene_objects.back()->set_reflectivity(.8f);
  scene_objects.push_back(new Triangle{{ 0, 0, 0},
                                       { 1, 1, 0},
                                       { 0, 1,-1}});
scene_objects.back()->set_color({0,0,255});
 scene_objects.back()->set_reflectivity(.8f);
  scene_objects.push_back(new Triangle{{ 0, 0, 0},
                                       { 0, 1, 1},
                                       { 1, 1, 0}});
scene_objects.back()->set_color({0,0,255});
 scene_objects.back()->set_reflectivity(.8f);
  /// Top half
  scene_objects.push_back(new Triangle{{ 0, 2, 0},
                                       { 0, 1, 1},
					 {-1, 1, 0}});
scene_objects.back()->set_color({0,0,255});
    scene_objects.back()->set_reflectivity(.8f);
  scene_objects.push_back(new Triangle{{ 0, 2, 0},
                                       { 1, 1, 0},
                                       { 0, 1, 1}});
  scene_objects.back()->set_color({0,0,255});
    scene_objects.back()->set_reflectivity(.8f);
  scene_objects.push_back(new Triangle{{ 0, 2, 0},
                                       { 0, 1,-1},
                                       { 1, 1, 0}});
  scene_objects.back()->set_color({0,0,255});
  scene_objects.back()->set_reflectivity(.8f);
  scene_objects.push_back(new Triangle{{ 0, 2, 0},
                                       {-1, 1, 0},
				       { 0, 1,-1}});
  scene_objects.back()->set_color({0,0,255});
  scene_objects.back()->set_reflectivity(.8f);
  
  Sphere* sphere1 = new Sphere(.3f); //creating a sphere object with .3f radius
  sphere1->set_pos({0.5f,1,-2});
  add_object_to_scene(sphere1, {0,255,0}, 0.0f);

  Sphere* sphere2 = new Sphere(.5f);
  sphere2->set_pos({-1,1.5f,-2});
  add_object_to_scene(sphere2, {255, 0, 0}, 0.7f);

  //Light initialization
  Vect light_pos = Vect{0,100,0};
  light_src = Light{light_pos}; //Initializing Light object with proper initial values
}


void export_color(Vect color) {
  /*
   * RVBA channels
   */
  img.push_back((std::max(0.f, std::min(255.f, color.x))+0.5));
  img.push_back((std::max(0.f, std::min(255.f, color.y))+0.5));
  img.push_back((std::max(0.f, std::min(255.f, color.z))+0.5));
  img.push_back(255);
}

bool propagate_ray(Vect* p_outgoing_ray_origin, Vect* p_outgoing_ray_dir, Vect* p_color){
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
    //No object in reach (case 1 : we hit the ground, case 2 : we "hit" the sky)
    if (ray_direction*Y<0) {
      hit_color =  ground_color(ray_direction, ray_origin);
    } else {
      hit_color = sky_color(ray_direction);
    }
    *p_color = *p_color + hit_color*ray_energy; //No object hit, draw sky or ground
    return false;
  }
}
float random_offset()
{
  // Random value uniform in [-0.5, 0.5]
  return static_cast<float>(rand()) / RAND_MAX - 0.5f;
}
int main() {
  Vect outgoing_ray_origin;
  Vect outgoing_ray_dir;
 
  init_scene(); //Initialize scene objects and light position
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

	//adding depth of field
	
	Vect sensor_shift{random_offset()*aperture,random_offset()*aperture,0.0f};
	ray_origin = ray_origin+sensor_shift;
	ray_direction = !(ray_direction - sensor_shift*(1./focal_distance));
	
        for(int number_bounces=0; number_bounces < max_hit_bounces; number_bounces++) {
          if(!propagate_ray(&outgoing_ray_origin, &outgoing_ray_dir, &color)){
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

  // Exporting file to .png using lodepng library
  const char* filename = "img.png";
  unsigned error = lodepng::encode(filename, img, cam.getWidth(), cam.getHeight());

  if(error) {
    cout << "encoder error" << error << ": "<< lodepng_error_text(error) << endl;
  } else {
    cout << "Exported img.png!\n";  
  }

  return 0;
}


