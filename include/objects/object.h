#ifndef OBJ_H
#define OBJ_H

#include "light.h"
#include "material/material.h"
#include "material/texture.h"
#include "vector.h"
#include <cmath>
#include <iostream>

class Object
{
  /**
   * Define a generic object class for different meshes that could appear on
   * screen
   */
public:
  Object() : material(){};

  virtual bool is_hit(const vect3 &incoming_ray_origin,
                      const vect3 &incoming_ray_dir, vect3 &outgoing_ray_origin,
                      vect3 &outgoing_ray_dir, float &hit_distance,
                      // vect3& hit_color
                      Material &hit_material, vect3 &hit_color, float ray_energy,
                      Light light_source) const
  {
    return false;
  };

  // Simplified version of is_hit : does not compute output
  virtual bool is_hit(const vect3 &incoming_ray_origin,
                      const vect3 &incoming_ray_dir) const
  {
    return false;
  };

  void setPos(const vect3 &new_pos) { pos = new_pos; }

  void set_color(const vect3 &v)
  {
    Texture *p_t = material.get_texture();
    p_t->setColor(v);
  }

  void set_texture(Texture *p_t) { material.set_texture(p_t); }

  // Useful for textured objects
  virtual vect2 map_3D_to_UV_coordinates(vect3 const &hit_point) const
  {
    return vect2{0.5, 0.5};
  };

  void set_reflectivity(const float &f) { material.reflectivity = f; }

  virtual Color computeColor(Material mat, Light light, vect3 hit_point, vect3 hit_normal,
                              vect3 out_ray_dir, vect3 object_center) const;

protected:
  Material material;
  vect3 pos;
};

#endif