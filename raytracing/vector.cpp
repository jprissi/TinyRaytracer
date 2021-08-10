#include <iostream>

#include "vector.h"

using namespace std;


Vect::Vect() : x(0), y(0), z(0) {}
Vect::Vect(double x, double y, double z) : x(x), y(y), z(z) {}

double Vect::getX() const { return x; }
double Vect::getY() const { return y; }
double Vect::getZ() const { return z; }

//Adding dot-product
double Vect::operator* (const Vect& a){
  return a.x*x + a.y*y + a.z*z;
}


