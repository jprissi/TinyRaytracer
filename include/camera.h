#ifndef CAM_H
#define CAM_H

#include "vector.h"
#include "matrix.hpp"

using namespace simple_matrix;

class Camera
{
private:
  vect3 position;
  matrix orientation;

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
      float aperture = 0.05f) : position(vect3{0, 0, 0}),
                                orientation(matrix(3, 3, {1, 0, 0, 0, 1, 0, 0, 0, 1})),
                                width(width),
                                height(height),
                                focalDistance(focalDistance),
                                aperture(aperture){};
  // Camera( int width, int height) : Camera(512, 512);
  int getWidth() const;
  int getHeight() const;

  void setPosition(vect3 const &new_pos) { position = new_pos; };
  vect3 getPosition() const { return position; };

  void setOrientation(matrix &new_ori) { this->orientation = new_ori; };
  matrix getOrientation() const { return this->orientation; };
};

#endif