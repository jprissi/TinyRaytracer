#include "scene.h"
#include "objects/sphere.h"
#include "objects/triangle.h"

void Scene::addObject(Object *obj, Vect color, float reflectivity) {
  // obj->set_pos(pos); //Not adapted to triangle object for now #TODO
  scene_objects.push_back(obj);
  scene_objects.back()->set_color(color);
  scene_objects.back()->set_reflectivity(reflectivity);
}

void Scene::init() {
  /*
   * Populate the scene with objects
   * Possible improvement : populate scene from .yaml file
   */

  addObject(new Triangle{{0, 0, 0}, {-1, 1, 0}, {0, 1, 1}}, {0, 0, 255}, .8f);
  addObject(new Triangle{{0, 0, 0}, {0, 1, -1}, {-1, 1, 0}}, {0, 0, 255}, .8f);
  addObject(new Triangle{{0, 0, 0}, {1, 1, 0}, {0, 1, -1}}, {0, 0, 255}, .8f);
  addObject(new Triangle{{0, 0, 0}, {0, 1, 1}, {1, 1, 0}}, {0, 0, 255}, .8f);
  /// Top half
  addObject(new Triangle{{0, 2, 0}, {0, 1, 1}, {-1, 1, 0}}, {0, 0, 255}, .8f);

  addObject(new Triangle{{0, 2, 0}, {1, 1, 0}, {0, 1, 1}}, {0, 0, 255}, .8f);
  addObject(new Triangle{{0, 2, 0}, {0, 1, -1}, {1, 1, 0}}, {0, 0, 255}, .8f);
  addObject(new Triangle{{0, 2, 0}, {-1, 1, 0}, {0, 1, -1}}, {0, 0, 255}, .8f);

  Sphere *sphere1 = new Sphere(.3f); // creating a sphere object with .3f radius
  sphere1->set_pos({0.5f, 1, -2});
  char texture_path[] = "assets/checker.png";
  ImageTexture *p_t = new ImageTexture(texture_path);

  // PlainTexture *p_t = new PlainTexture();
  sphere1->set_texture(p_t);
  addObject(sphere1, {0, 255, 0}, 0.0f);

  Sphere *sphere2 = new Sphere(.5f);
  sphere2->set_pos({-1, 1.5f, -2});
  addObject(sphere2, {255, 0, 0}, 0.7f);

  // Light initialization
  Vect light_pos = Vect{0, 100, 0};
  lightSource =
      Light{light_pos}; // Initializing Light object with proper initial values
}