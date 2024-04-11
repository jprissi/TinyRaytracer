#ifndef TEXTURE_H
#define TEXTURE_H

#include "io/imageloader.h"
#include "vector.h"

class Texture
{
public:
  void setColor(Color const &color) { this->color = color; };
  virtual Color getColor(vect2 const &hit_point,
                         vect3 const &object_position) const { return this->color; };

protected:
  Color color;
};

class ImageTexture : public Texture
{
public:
  ImageTexture(const char *texture_path);
  Color getColor(vect2 const &hit_point,
                 vect3 const &object_position) const override;
  void getTextureSize(uint &xsize_out, uint &ysize_out) const
  {
    xsize_out = this->xsize;
    ysize_out = this->ysize;
  };

private:
  Color getTexturePixel(uint const &x, uint const &y) const;
  std::vector<unsigned char> texture_image;
  unsigned xsize, ysize = 0;
  std::string texture_path;
};

#endif