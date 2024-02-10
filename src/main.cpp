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

/**
 * This project was written while following the slides for the "Let's build a
 * simple raytracer" course available on Github.
 * I took the liberty of adapting the project architecture.
 */

Scene scene;
Camera camera = Camera();
std::vector<unsigned char> img;

int main()
{
  Raytracer(camera).compute(img);
  return 0;
}
