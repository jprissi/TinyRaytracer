#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "light.h"
#include "objects/object.h"
#include "stdio.h"
#include <vector>

class Scene {
public:
  // Container for the objects of the scene
  std::vector<Object *> scene_objects;
  Light lightSource;
  Camera camera;
  void addObject(Object *obj, vect3 color, float reflectivity);
  // void setCamera(Camera c) : camera(c) {};
  void init();
};

#endif