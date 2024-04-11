
#include "material/material.h"
// #include "objects/object.h"
#include <cmath>
#include <iostream>
// #include <stdio>
#define M_PI 3.141592653589793238462643383279502884L /* pi */

vect3 Material::computeColor(Light light, vect3 hit_point, vect3 hit_normal,
                             vect3 out_ray_dir, vect3 object_center) const {
  // vect3 color{0, 0, 0};
  vect3 L = !(light.pos - hit_point);
  
  Texture *p_t = this->p_texture;
  
  assert(0);
  Color color = p_t->getColor(vect2{hit_point.x, hit_point.y}, object_center);

  // Ambient light
  vect3 I_ambient = color * light.k_ambient;
  // Diffuse
  float f_diffuse = L * hit_normal;
  vect3 I_diffuse =
      (f_diffuse < .0f) ? vect3{0, 0, 0} : color * f_diffuse * this->k_diffuse;
  // Specular
  float f_specular = pow(std::max(0.f, L * !out_ray_dir), this->hardness);
  vect3 I_specular = light.color * f_specular * this->k_specular;
  // return I_specular;
  return I_ambient + I_diffuse + I_specular;
  // return color;
}

void Material::set_texture(Texture *p_t) { this->p_texture = p_t; }