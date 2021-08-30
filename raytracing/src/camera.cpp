#include <iostream>

#include "vector.h"

using namespace std;

class Camera {
 private:
  Vect pos;
  Vect dir;

  //Temp variables
  int projection_center;
  int focal_length;
  int width, height;
public:
  Camera(): Camera(512, 512) {};
  Camera(int width, int height) : width(width), height(height), projection_center(0), focal_length(-1), pos(Vect{0,0,0}), dir(Vect{0,0,1}) { };
};
