#ifndef OBJ_H
#define OBJ_H

#include "light.h"
#include "material/material.h"
#include "vector.h"
#include <cmath>
#include <iostream>

class Object {
  /**
   * Define a generic object class for different meshes that could appear on
   * screen
   */
public:
  Object() : material(){};
  Material material;
  vect3 pos;
  virtual bool is_hit(const vect3 &incoming_ray_origin,
                      const vect3 &incoming_ray_dir, vect3 &outgoing_ray_origin,
                      vect3 &outgoing_ray_dir, float &hit_distance,
                      // vect3& hit_color
                      Material &hit_material, vect3 &hit_color, float ray_energy,
                      Light light_source) const = 0;

  // Simplified version of is_hit : does not compute output
  virtual bool is_hit(const vect3 &incoming_ray_origin,
                      const vect3 &incoming_ray_dir) const = 0;

  void set_pos(const vect3 &new_pos) { pos = new_pos; }

  void set_color(const vect3 &v) { material.p_texture->color = v; }

  void set_texture(Texture *p_t) { material.p_texture = p_t; }

  void set_reflectivity(const float &f) { material.reflectivity = f; }

  vect3 compute_color(Material mat, Light light, vect3 hit_point, vect3 hit_normal,
                     vect3 out_ray_dir, vect3 object_center) const;
};

#endif