#ifndef TEXTURE_H
#define TEXTURE_H

#include "io/imageloader.h"
#include "vector.h"

class Texture {
public:
  virtual vect3 get_color(vect3 const &hit_point,
                          vect3 const &object_position) const = 0;

  vect3 color;
};

class ImageTexture : public Texture {
public:
  virtual vect3 get_color(vect3 const &hit_point,
                          vect3 const &object_position) const;
  ImageTexture(const char *texture_path) {
    texture_image = ImageLoader::loadPNG(texture_path, xsize, ysize);
    std::cout << xsize << "; " << ysize << std::endl;
  };

  std::vector<unsigned char> texture_image;
  unsigned xsize, ysize;
  std::string texture_path;

  // First, attempt to implement onto sphere
  vect3 map_3D_to_UV_coordinates(vect3 const &hit_point,
                                 vect3 const &object_position) const;
  vect3 get_pixel(unsigned const x, unsigned const y) const;
};

class PlainTexture : public Texture {
public:
  virtual vect3 get_color(vect3 const &hit_point,
                          vect3 const &object_position) const;
};

#endif