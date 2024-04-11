#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

#include "camera.h"
#include "light.h"
#include "lodepng.h"
#include "objects/object.h"
#include "ray.hpp"
#include "scene.h"
#include "vector.h"
#include "raytracer.h"
#include "matrix.hpp"

using namespace simple_matrix;

/**
 * This project was written while following the slides for the "Let's build a
 * simple raytracer" course available on Github.
 * I took the liberty of adapting the project architecture.
 */

Scene scene;
uint s = 256;
Camera camera = Camera(2 * s, s, 2.5, 0.05);

std::vector<unsigned char> img;

matrix rotationMatrix(float x, float y, float z)
{
  matrix rotation(3, 3, {1, 0, 0, 0, 1, 0, 0, 0, 1});

  if (z != 0)
  {
    float c = std::cos(z);
    float s = std::sin(z);
    // float val = {1, 0, 0, 0, c, -s, 0, s, c};
    matrix r(3, 3);
    r(0, 0) = c;
    r(0, 1) = -s;
    r(1, 0) = s;
    r(1, 1) = c;
    r(2, 2) = 1;
    rotation *= r;
  }
  if (y != 0)
  {
    float c = std::cos(y);
    float s = std::sin(y);
    // float val = {1, 0, 0, 0, c, -s, 0, s, c};
    matrix r(3, 3);
    r(0, 0) = c;
    r(0, 2) = s;
    r(1, 1) = 1;
    r(2, 0) = -s;
    r(2, 2) = c;
    rotation *= r;
  }
  if (x != 0)
  {
    float c = std::cos(x);
    float s = std::sin(x);
    // float val = {1, 0, 0, 0, c, -s, 0, s, c};
    matrix r1(3, 3);
    r1(0, 0) = 1.0;
    r1(1, 1) = c;
    r1(1, 2) = -s;
    r1(2, 1) = s;
    r1(2, 2) = c;
    rotation *= r1;
  }
  return rotation;
}

int main()
{
  float x, y, z;

  matrix rotation = rotationMatrix(45*M_PI/360, 10*M_PI/360, 50*M_PI/360);
  std::cout << rotation << std::endl;
  camera.setPosition(vect3(0, 1, 0));
  camera.setOrientation(rotation);

  Raytracer(&scene).compute(img, camera);
  return 0;
}
