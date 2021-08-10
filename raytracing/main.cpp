#include <iostream>
#include <fstream>

#include "vector.h"

using namespace std;

int main() {
  // Creating a ppm image file
  std::ofstream outfile("img.ppm");
  outfile << "P3 512 512 255";

  for (int i=0; i<512; i++){
    for (int i=0; i<512; i++){
      outfile << " " << 0;
      outfile << " " << 0;
      outfile << " " << 255;
    }
  }
  
  Vec3d i(1, 0, 0);

  cout << i.getX();
  cout << "\n";


  cout << "Exported img.ppm!\n";  
  return 0;
}
