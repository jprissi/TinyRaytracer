#include "scene.h"
#include "objects/sphere.h"
#include "objects/triangle.h"
#include <cassert>

void Scene::addObject(Object *obj, vect3 color, float reflectivity)
{
  // obj->setPos(pos); //Not yet suitable for triangles (#TODO)
  scene_objects.push_back(obj);
  scene_objects.back()->set_color(color);
  scene_objects.back()->set_reflectivity(reflectivity);
}

void Scene::init()
{
  /*
   * Populate the scene with objects
   * Possible improvement : populate scene from .yaml file (#TODO)
   */

  // Octahedron
  addObject(new Triangle{{0, 0, 0}, {-1, 1, 0}, {0, 1, 1}}, {0, 0, 255}, .8f);
  addObject(new Triangle{{0, 0, 0}, {0, 1, -1}, {-1, 1, 0}}, {0, 0, 255}, .8f);
  addObject(new Triangle{{0, 0, 0}, {1, 1, 0}, {0, 1, -1}}, {0, 0, 255}, .8f);
  addObject(new Triangle{{0, 0, 0}, {0, 1, 1}, {1, 1, 0}}, {0, 0, 255}, .8f);
  /// Top half
  addObject(new Triangle{{0, 2, 0}, {0, 1, 1}, {-1, 1, 0}}, {0, 0, 255}, .8f);
  addObject(new Triangle{{0, 2, 0}, {1, 1, 0}, {0, 1, 1}}, {0, 0, 255}, .8f);
  addObject(new Triangle{{0, 2, 0}, {0, 1, -1}, {1, 1, 0}}, {0, 0, 255}, .8f);
  addObject(new Triangle{{0, 2, 0}, {-1, 1, 0}, {0, 1, -1}}, {0, 0, 255}, .8f);

  char texture_path[] = "assets/checker.png";
  ImageTexture *p_texture = new ImageTexture(texture_path);
  uint x, y;
  p_texture->getTextureSize(x,y);
  assert(x>0);
  assert(y>0);

  Sphere *sphere1 = new Sphere(.3f); // creating a sphere object with .3f radius
  sphere1->setPos({0.5f, 1, -2});
  sphere1->set_texture(p_texture);
  addObject(sphere1, {0, 255, 0}, 0.0f);

  Sphere *sphere2 = new Sphere(.5f);
  sphere2->setPos({-1, 1.5f, -2});
  // sphere2->set_texture(p_texture);
  addObject(sphere2, {255, 0, 0}, 0.7f);

  char plank_texture_path[] = "assets/textures/raw_plank_wall_diff_1k.png";
  ImageTexture *p_plank_texture = new ImageTexture(plank_texture_path);
  this->ground_texture = p_plank_texture;

  // Light initialization
  vect3 light_pos = vect3{0, 100, 0};
  lightSource =
      Light{light_pos}; // Initializing Light object with proper initial values
  
}