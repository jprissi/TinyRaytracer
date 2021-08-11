#include <iostream>
#include <cmath>
#include "vector.h"

Vect::Vect() : x(0), y(0), z(0) {}
Vect::Vect(double x, double y, double z) : x(x), y(y), z(z) {}

double Vect::getX() const { return x; }
double Vect::getY() const { return y; }
double Vect::getZ() const { return z; }

//Adding dot-product
double Vect::operator* (const Vect& a){
  return a.x*x + a.y*y + a.z*z;
}
Vect Vect::operator+ (const Vect& a){
  return Vect{a.x+x, a.y+y, a.z+z};
}
Vect Vect::operator- (const Vect& a){
  return Vect{x-a.x, y-a.y, z-a.z};
}
Vect Vect::operator*(double k) const {
  return Vect{k*x, k*y, k*z};
}

double Vect::length() const
{
  return std::sqrt(x*x + y*y + z*z);
}
Vect Vect::operator!() const {
  return (*this)*(1.d/length());
}



