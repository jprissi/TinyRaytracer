#ifndef VEC_H
#define VEC_H

using namespace std;

class Vect {
 public:
  Vect();
  Vect(double x, double y, double z);

  double getX() const;
  double getY() const;
  double getZ() const;

 private:
  double x,y,z;
};

#endif
