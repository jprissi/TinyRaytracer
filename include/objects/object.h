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
  Vect pos;
  virtual bool is_hit(const Vect &incoming_ray_origin,
                      const Vect &incoming_ray_dir, Vect &outgoing_ray_origin,
                      Vect &outgoing_ray_dir, float &hit_distance,
                      // Vect& hit_color
                      Material &hit_material, Vect &hit_color, float ray_energy,
                      Light light_source) const = 0;

  // Simplified version of is_hit : does not compute output
  virtual bool is_hit(const Vect &incoming_ray_origin,
                      const Vect &incoming_ray_dir) const = 0;

  void set_pos(const Vect &new_pos) { pos = new_pos; }

  void set_color(const Vect &v) { material.p_texture->color = v; }

  void set_texture(Texture *p_t) { material.p_texture = p_t; }

  void set_reflectivity(const float &f) { material.reflectivity = f; }

  Vect compute_color(Material mat, Light light, Vect hit_point, Vect hit_normal,
                     Vect out_ray_dir, Vect object_center) const;
};

#endif