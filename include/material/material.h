#ifndef MAT_H
#define MAT_H

#include "light.h"
#include "material/texture.h"
#include "vector.h"

class Material {
  /**
   * Defines a basic material
   */
public:
  Material() : reflectivity(0) { p_texture = new PlainTexture(); };
  //~Material() { delete p_texture; };
  // constexpr Material(const Material &m)
  //     : reflectivity(m.reflectivity), p_texture(m.p_texture){};
  Texture *p_texture;
  float reflectivity;
  float k_diffuse = 0.4f;
  float k_specular = 0.4f;
  float hardness = 100.0f;

  Vect compute_color(Light light, Vect hit_point, Vect hit_normal,
                     Vect out_ray_dir, Vect object_center) const;

  void set_texture(Texture *p_t);
};

#endif