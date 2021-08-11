#ifndef VEC_H
#define VEC_H

using namespace std;

class Vect {
 public:
  Vect();
  Vect(double x, double y, double z);

  //Copy vector
  //Vect(const Vect& v);
  //Adding dot-product operation
  double operator* (const Vect& a);
  //Vector sum operation
  Vect operator+ (const Vect& a);
  Vect operator- (const Vect& a);
  Vect operator* (double k) const;
  Vect operator!() const;
  
  double getX() const;
  double getY() const;
  double getZ() const;

  double length() const;
 private:
  double x,y,z;
};

#endif
