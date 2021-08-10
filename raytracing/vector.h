#ifndef VEC_H
#define VEC_H

using namespace std;

class Vect {
 public:
  Vect();
  Vect(double x, double y, double z);

  //Copy vector
  Vect(const Vect& v);
  //Adding dot-product operation
  double operator* (const Vect& a);
  
  double getX() const;
  double getY() const;
  double getZ() const;

 private:
  double x,y,z;
};

#endif
