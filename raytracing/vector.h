#ifndef VEC_H
#define VEC_H

#include <iostream>
using namespace std;

class Vec3d {
 public:
  Vec3d();
  Vec3d(double x, double y, double z);

  double getX() const;
  double getY() const;
  double getZ() const;

 private:
  double x,y,z;
};

#endif
