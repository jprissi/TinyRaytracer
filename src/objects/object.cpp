#include "objects/object.h"
#include "material/material.h"

vect3 Object::computeColor(Material mat, Light light, vect3 hit_point,
                           vect3 hit_normal, vect3 out_ray_dir,
                           vect3 object_center) const {

  return mat.computeColor(light, hit_point, hit_normal, out_ray_dir,
                           object_center);
}