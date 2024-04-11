#ifndef VEC_H
#define VEC_H

#include <ostream>

class vect3
{
public:
  // Constructors
  vect3() : x(0), y(0), z(0){};
  vect3(float x, float y, float z) : x(x), y(y), z(z){};
  // vect3(const vect3 &v) : x(v.x), y(v.y), z(v.z){};
  float x, y, z;
  float operator*(const vect3 &v) const; // dot product
  vect3 operator+(const vect3 &v) const;
  vect3 operator-(const vect3 &v) const;
  vect3 operator^(const vect3 &v) const; // cross product
  vect3 operator*(float k) const;
  vect3 operator+=(const vect3 &v);
  vect3 operator!() const; // normalization
  float length() const;
};

std::ostream &operator<<(std::ostream &strm, const vect3 &v);

typedef vect3 Color; // Simple alias

class vect2
{
public:
  vect2() : x(0.), y(0.){};
  vect2(float x, float y) : x(x), y(y){};

  float operator*(const vect2 &v) const; // dot product
  vect2 operator+(const vect2 &v) const;
  vect2 operator-(const vect2 &v) const;
  // vect2 operator^(const vect2 &v) const; // cross product
  vect2 operator*(float k) const;
  // vect2 operator!() const; // normalization
  // float length() const;
  float x, y;
};

std::ostream &operator<<(std::ostream &strm, const vect2 &v);

#endif