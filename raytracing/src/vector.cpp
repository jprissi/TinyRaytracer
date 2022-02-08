#include <iostream>
#include <cmath>
#include "vector.h"

Vect::Vect() : x(0), y(0), z(0) {}
Vect::Vect(float x, float y, float z) : x(x), y(y), z(z) {}
//copy constructor
Vect::Vect(const Vect& v) : x(v.x), y(v.y), z(v.z) { }
float Vect::getX() const { return x; }
float Vect::getY() const { return y; }
float Vect::getZ() const { return z; }

//Adding dot-product
float Vect::operator* (const Vect& a) const {
  return a.x*x + a.y*y + a.z*z;
}
Vect Vect::operator+ (const Vect& a) const {
  return Vect{a.x+x, a.y+y, a.z+z};
}
Vect Vect::operator- (const Vect& a) const {
  return *this + a*-1;
}
Vect Vect::operator*(float k) const {
  return Vect{k*x, k*y, k*z};
}
Vect Vect::operator^(const Vect& v) const {
  return Vect{
    y*v.z - z*v.y,
    z*v.x - x*v.z,
    x*v.y - y*v.x
  };
}


float Vect::length() const
{
  return std::sqrt(x*x + y*y + z*z);
}
Vect Vect::operator!() const {
  
  return (*this)*(1.f/length());
}

std::ostream& operator<<(std::ostream &strm, const Vect &v) {
  return strm << "(" << v.x << "; " << v.y << "; " << v.z << ")";
}

