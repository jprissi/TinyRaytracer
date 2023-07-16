#include "material/texture.h"
#include <cmath>

Vect PlainTexture::get_color(Vect const &hit_point,
                             Vect const &object_center) const {
  return this->color;
}

Vect ImageTexture::get_color(Vect const &hit_point,
                             Vect const &object_center) const {
  Vect color = this->map_to_UV_coordinates(hit_point, object_center);
  return color;
}

Vect ImageTexture::map_to_UV_coordinates(Vect const &hit_point,
                                         Vect const &object_center) const {
  // For Sphere:

  Vect d = !(hit_point - object_center);
  float u = 0.5 + std::atan2(d.z, d.x) / (2 * M_PI);
  float v = 0.5 + std::asin(d.y) / M_PI;

  unsigned x = (int)(std::round(u * (this->xsize - 1)) + 0.5);
  x = std::min(x, this->xsize - 1);
  unsigned y = (int)(std::round(v * (this->ysize - 1)) + 0.5);
  y = std::min(y, this->ysize - 1);

  return ImageTexture::get_pixel(x, y);
}

Vect ImageTexture::get_pixel(unsigned const x, unsigned const y) const {

  unsigned idx = (unsigned int)(this->texture_image.size() - 1);
  idx = std::min(y * this->xsize + x, idx);

  unsigned char r = this->texture_image[4 * idx + 0];
  unsigned char g = this->texture_image[4 * idx + 1];
  unsigned char b = this->texture_image[4 * idx + 2];
  unsigned char a = this->texture_image[4 * idx + 3];

  return Vect(r, g, b);
}