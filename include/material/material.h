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
  Texture *p_texture;
  float reflectivity;
  float k_diffuse = 0.4f;
  float k_specular = 0.4f;
  float hardness = 100.0f;

  Material() : reflectivity(0) { p_texture = new PlainTexture(); };

  vect3 compute_color(Light light, vect3 hit_point, vect3 hit_normal,
                      vect3 out_ray_dir, vect3 object_center) const;

  void set_texture(Texture *p_t);
};

#endif