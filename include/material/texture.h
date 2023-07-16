#ifndef TEXTURE_H
#define TEXTURE_H

#include "lodepng.h"
#include "vector.h"
#include <iostream>
#include <vector>

class Texture {
public:
  // Texture(Vect color) : color(color){};
  // Texture() : color(0, 0, 0){};

  virtual Vect get_color(Vect const &hit_point,
                         Vect const &object_center) const = 0;

  Vect color;
};

class Loader {
public:
  static std::vector<unsigned char> loadPNG(const char *filename,
                                            unsigned &width, unsigned &height) {
    return decodeTwoSteps(filename, width, height);
  }

  static std::vector<unsigned char>
  decodeTwoSteps(const char *filename, unsigned &width, unsigned &height) {
    std::vector<unsigned char> png;
    std::vector<unsigned char> image; // the raw pixels
    // unsigned width, height;

    // load and decode
    unsigned error = lodepng::load_file(png, filename);
    if (!error)
      error = lodepng::decode(image, width, height, png);

    // if there's an error, display it
    if (error)
      std::cout << "decoder error " << error << ": "
                << lodepng_error_text(error) << std::endl;

    // the pixels are now in the vector "image", 4 bytes per pixel, ordered
    // RGBARGBA..., use it as texture, draw it, ...
    return image;
  }
};

class ImageTexture : public Texture {
public:
  virtual Vect get_color(Vect const &hit_point,
                         Vect const &object_center) const;
  ImageTexture(const char *texture_path) {
    texture_image = Loader::loadPNG(texture_path, xsize, ysize);
    std::cout << xsize << "; " << ysize << std::endl;
  };

  std::vector<unsigned char> texture_image;
  unsigned xsize, ysize;
  std::string texture_path;

  // First, attempt to implement onto sphere
  Vect map_to_UV_coordinates(Vect const &hit_point,
                             Vect const &object_center) const;
  Vect get_pixel(unsigned const x, unsigned const y) const;
};

class PlainTexture : public Texture {
public:
  virtual Vect get_color(Vect const &hit_point,
                         Vect const &object_center) const;
};

#endif