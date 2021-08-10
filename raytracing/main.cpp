#include <iostream>
#include <fstream>

//#include "vector.h"
#include "vector.h"
#include "camera.cpp"

using namespace std;

string ray(double x, double y) {

  if (y<0) {
    //hitting ground
    return "0 0 0";
  }
  if(y>=0){
    //"hitting" sky
    return "255 255 255";
  }
}

int main() { 
  const Vect X{1, 0, 0};
  const Vect Y{0, 1, 0};
  const Vect Z{0, 0, 1};

  Camera cam{512, 512};
  
  Vect ray_origin{0,1,-4};

  // double v = ray_origin*Z;
  //cout << "origin*Z = " << v;

  // Testing dot product
  // cout << "origin(" << ray_origin*X << ", " << ray_origin*Y << ", " << ray_origin*Z << ")\n";


  
  // Creating a ppm image file
  ofstream outfile("img.ppm");
  outfile << "P3 512 512 255";

  //Left-handed base with z forward
  for (int y=256; y>=-255; y--){
    for (int x=-255; x<= 256; x++){
      //For each image pixel...
      //We use pixel centers (x-0.5), y(0.5)
      // cout << " " << ray(x,y);
      outfile << " " << ray(x, y);
      Vector ray_direction = !Vector{
    }
    // cout << "\n";
  }

  cout << "Exported img.ppm!\n";  
  return 0;
}


