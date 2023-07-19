#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray {
public:
  Color initColor;
  Color finalColor;
  vect3 origin;
  vect3 direction;

  float energy = 1.f;
};

#endif