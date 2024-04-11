#include "vector.h"
#include <cmath>
#include <iostream>

// Adding dot-product
float vect3::operator*(const vect3 &a) const
{
  return a.x * x + a.y * y + a.z * z;
}
vect3 vect3::operator+(const vect3 &a) const
{
  return vect3{a.x + x, a.y + y, a.z + z};
}
vect3 vect3::operator-(const vect3 &a) const { return *this + a * -1; }
vect3 vect3::operator*(float k) const { return vect3{k * x, k * y, k * z}; }
vect3 vect3::operator^(const vect3 &v) const
{
  return vect3{y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
}
vect3 vect3::operator+=(const vect3 &v)
{
  x += v.x;
  y += v.y;
  z += v.z;
  return *this; // Return reference to current object for chaining
}
vect3 vect3::operator!() const { return (*this) * (1.f / length()); }
float vect3::length() const { return std::sqrt(x * x + y * y + z * z); }

std::ostream &operator<<(std::ostream &strm, const vect3 &v)
{
  return strm << "(" << v.x << "; " << v.y << "; " << v.z << ")";
}

vect2 vect2::operator+(const vect2 &a) const
{
  return vect2{a.x + x, a.y + y};
}
vect2 vect2::operator-(const vect2 &a) const { return *this + a * -1; }
vect2 vect2::operator*(float k) const { return vect2{x * k, y * k}; }
std::ostream &operator<<(std::ostream &strm, const vect2 &v)
{
  return strm << "(" << v.x << "; " << v.y << ")";
}