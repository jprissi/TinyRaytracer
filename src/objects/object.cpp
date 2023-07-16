#include "objects/object.h"
#include "material/material.h"

Vect Object::compute_color(Material mat, Light light, Vect hit_point,
                           Vect hit_normal, Vect out_ray_dir,
                           Vect object_center) const {

  return mat.compute_color(light, hit_point, hit_normal, out_ray_dir,
                           object_center);
}