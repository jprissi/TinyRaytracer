#ifndef VEC_H
#define VEC_H

class Vect {
 public:
  //Constructors
  Vect();
  Vect(float x, float y, float z);
  Vect(const Vect& v);
  //Adding dot-product operation
  float operator* (const Vect& v) const;
  //Vector sum operation
  Vect operator+ (const Vect& v) const;
  Vect operator- (const Vect& v) const;
  Vect operator^ (const Vect& v) const;
  Vect operator* (float k) const;
  Vect operator!() const;
  
  float getX() const;
  float getY() const;
  float getZ() const;

  float length() const;

  float x,y,z;
};

#endif
