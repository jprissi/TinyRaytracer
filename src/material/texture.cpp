#include "material/texture.h"
#include <cmath>
#include <cassert>

ImageTexture::ImageTexture(const char *texture_path)
{
  texture_image = ImageLoader::loadPNG(texture_path, xsize, ysize);
  assert(xsize > 0);
  assert(ysize > 0);
};

Color ImageTexture::getColor(vect2 const &hit_point,
                             vect3 const &object_position) const
{
  Color out_color; // = this->color;
  // if (p_parent_object == nullptr)
  // {
  //   std::cerr << "No parent object set for texture, using default color!" << std::endl;
  //   return out_color;
  // }

  float alpha = 0.5;
  Color texture_color = getTexturePixel(std::round(hit_point.x), std::round(hit_point.y));
  // std::cout << texture_color << std::endl;
  out_color = (texture_color - out_color) * alpha + out_color; // Mix base color with texture

  return out_color;
}

vect3 ImageTexture::getTexturePixel(uint const &x_in, uint const &y_in) const
{
  /* Retrieves pixel (x, y) from texture */
  assert(texture_image.size() > 0);
  assert(xsize > 0);
  assert(ysize > 0);

  // wrap
  uint y = y_in % ysize;
  uint x = x_in % xsize;
  uint idx = y * this->xsize + x; // convert

  // clip
  // uint max_idx = (unsigned int)(this->texture_image.size() - 1);
  // idx = std::min(max_idx, idx);

  unsigned char r = this->texture_image[4 * idx + 0];
  unsigned char g = this->texture_image[4 * idx + 1];
  unsigned char b = this->texture_image[4 * idx + 2];
  // unsigned char a = this->texture_image[4 * idx + 3]; // Transparency channel (unused)

  return vect3(r, g, b);
}