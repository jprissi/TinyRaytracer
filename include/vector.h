#ifndef VEC_H
#define VEC_H

#include <ostream>


class Vect {
 public:
  //Constructors
  Vect() : x(0), y(0), z(0) {};
  Vect(float x, float y, float z) : x(x), y(y), z(z) {};
  Vect(const Vect& v): x(v.x), y(v.y), z(v.z) {};
  float x,y,z;
  float operator* (const Vect& v) const; //dot product
  Vect operator+ (const Vect& v) const;
  Vect operator- (const Vect& v) const;
  Vect operator^ (const Vect& v) const; //cross product
  Vect operator* (float k) const;
  Vect operator!() const; //normalization
  float length() const;
};
std::ostream& operator<<(std::ostream &strm, const Vect &v);

typedef Vect Color; //Simple alias

#endif
