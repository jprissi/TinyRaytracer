#ifndef CAM_H
#define CAM_H

#include "vector.h"

class Camera
{
private:
  Vect position;
  Vect direction;
  // Temp variables
  //  int projection_center;
  //  int focal_length;
  int width, height;

public:
  float focalDistance;
  float aperture;
  // Camera() : Camera(512, 512){};
  Camera(
      int width = 512,
      int height = 512,
      float focalDistance = 2.5f,
      float aperture = 0.05f) : position(Vect{0, 0, 0}),
                                direction(Vect{0, 0, 1}),
                                width(width),
                                height(height),
                                focalDistance(focalDistance),
                                aperture(aperture){};
  // Camera( int width, int height) : Camera(512, 512);
  int getWidth() const;
  int getHeight() const;
};

#endif