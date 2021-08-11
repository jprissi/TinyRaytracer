#include <iostream>
#include <fstream>
#include <cmath>

//#include "vector.h"
#include "vector.h"
#include "camera.cpp"

using namespace std;


  const Vect X{1, 0, 0};
  const Vect Y{0, 1, 0};
  const Vect Z{0, 0, 1};

Vect sky_color(Vect direction) {
  cout << direction*Y << "\n";
  return Vect{0.7, 0.6, 1.0}*255.d * std::pow(1-direction*Y, 2);
}

Vect ray(Vect direction, Vect origin) {

  if (direction*Y<0) {
    //hitting ground
    return Vect{0, 0, 0};
  }
  if(direction*Y>=0){
    //"hitting" sky
    return sky_color(direction);
  }
}

int main() { 

  Camera cam{512, 512};
  
  Vect ray_origin{0,1,-4};
  Vect ray_direction{0, 0, 0};

  Vect color{0,0,0};
  // double v = ray_origin*Z;
  //cout << "origin*Z = " << v;

  // Testing dot product
  // cout << "origin(" << ray_origin*X << ", " << ray_origin*Y << ", " << ray_origin*Z << ")\n";


  
  // Creating a ppm image file
  ofstream outfile("img.ppm");
  outfile << "P3 512 512 255 ";

  //Left-handed base with z forward
  for (int y=256; y>=-255; y--){
    for (int x=-255; x<= 256; x++){
      //For each image pixel...
      //We use pixel centers (x-0.5), (y-0.5)
      // cout << " " << ray(x,y);
      ray_direction = !(Vect{x-0.5, y-0.5, 0} - ray_origin);
      //cout << "dir(" << ray_direction*X << ", " << ray_direction*Y << ", " << ray_direction*Z << ")\n";

      color = ray(ray_direction, ray_origin);
      //cout << "color(R=" << color*X << ", V=" << color*Y << ", B=" << color*Z << ")\n";
      outfile
	<<  (int)(std::max(0.d, std::min(255.d, color.getX()))+0.5)
	<< " "
	<<  (int)(std::max(0.d, std::min(255.d, color.getY()))+0.5)
	<< " "
	<< (int)(std::max(0.d, std::min(255.d, color.getZ()))+0.5) //static_cast<unsigned char>()
	<< " ";
    }
    // cout << "\n";
  }

  cout << "Exported img.ppm!\n";  
  return 0;
}


