#include "material/texture.h"
#include <cmath>

vect3 PlainTexture::get_color(vect3 const &hit_point,
                              vect3 const &object_position) const {
  return this->color;
}

vect3 ImageTexture::get_color(vect3 const &hit_point,
                              vect3 const &object_position) const {
  vect3 color = this->map_3D_to_UV_coordinates(hit_point, object_position);
  return color;
}

vect3 ImageTexture::map_3D_to_UV_coordinates(
    vect3 const &hit_point, vect3 const &object_position) const {
  // For Sphere:

  vect3 d = !(hit_point - object_position);
  float u = 0.5 + std::atan2(d.z, d.x) / (2 * M_PI);
  float v = 0.5 + std::asin(d.y) / M_PI;

  unsigned x = (int)(std::round(u * (this->xsize - 1)) + 0.5);
  x = std::min(x, this->xsize - 1);
  unsigned y = (int)(std::round(v * (this->ysize - 1)) + 0.5);
  y = std::min(y, this->ysize - 1);

  return ImageTexture::get_pixel(x, y);
}

vect3 ImageTexture::get_pixel(unsigned const x, unsigned const y) const {

  unsigned idx = (unsigned int)(this->texture_image.size() - 1);
  idx = std::min(y * this->xsize + x, idx);

  unsigned char r = this->texture_image[4 * idx + 0];
  unsigned char g = this->texture_image[4 * idx + 1];
  unsigned char b = this->texture_image[4 * idx + 2];
  unsigned char a = this->texture_image[4 * idx + 3];

  return vect3(r, g, b);
}