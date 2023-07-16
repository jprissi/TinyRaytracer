#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray {
public:
  Color initColor;
  Color finalColor;
  Vect origin;
  Vect direction;

  float energy = 1.f;
};

#endif