#include <iostream>
#include <cmath>
#include "vector.h"

//Adding dot-product
float Vect::operator*(const Vect &a) const
{
  return a.x * x + a.y * y + a.z * z;
}
Vect Vect::operator+(const Vect &a) const
{
  return Vect{a.x + x, a.y + y, a.z + z};
}
Vect Vect::operator-(const Vect &a) const
{
  return *this + a * -1;
}
Vect Vect::operator*(float k) const
{
  return Vect{k * x, k * y, k * z};
}
Vect Vect::operator^(const Vect &v) const
{
  return Vect{
      y * v.z - z * v.y,
      z * v.x - x * v.z,
      x * v.y - y * v.x};
}
Vect Vect::operator!() const
{
  return (*this) * (1.f / length());
}
float Vect::length() const
{
  return std::sqrt(x * x + y * y + z * z);
}

std::ostream &operator<<(std::ostream &strm, const Vect &v)
{
  return strm << "(" << v.x << "; " << v.y << "; " << v.z << ")";
}