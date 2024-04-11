#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include "lodepng.h"
#include <iostream>
#include <vector>
#include <cassert>

class ImageLoader
{
public:
  static std::vector<unsigned char> loadPNG(const char *filename,
                                            unsigned &width, unsigned &height)
  {
    return decodeTwoSteps(filename, width, height);
  }

  // Code sample provided by lodepng
  static std::vector<unsigned char>
  decodeTwoSteps(const char *filename, unsigned &width, unsigned &height)
  {
    std::vector<unsigned char> png;
    std::vector<unsigned char> image; // the raw pixels
    // unsigned width, height;

    // load and decode
    unsigned error = lodepng::load_file(png, filename);
    if (!error)
      error = lodepng::decode(image, width, height, png);

    // if there's an error, display it and stop execution
    if (error)
    {
      std::cout << "decoder error " << error << ": "
                << lodepng_error_text(error) << std::endl
                << std::flush;
      assert(0);
    }
    // the pixels are now in the vector "image", 4 bytes per pixel, ordered
    // RGBARGBA..., use it as texture, draw it, ...
    return image;
  }
};

#endif