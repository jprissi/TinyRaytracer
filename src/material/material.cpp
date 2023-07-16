
#include "material/material.h"
// #include "objects/object.h"
#include <cmath>
#include <iostream>
// #include <stdio>
#define M_PI 3.141592653589793238462643383279502884L /* pi */

Vect Material::compute_color(Light light, Vect hit_point, Vect hit_normal,
                             Vect out_ray_dir, Vect object_center) const {
  // Vect color{0, 0, 0};
  Vect L = !(light.pos - hit_point);
  // Ambient light
  Texture *p_t = this->p_texture;
  Vect color = p_t->get_color(hit_point, object_center);
  Vect I_ambient = color * light.k_ambient;
  // Diffuse
  float f_diffuse = L * hit_normal;
  Vect I_diffuse =
      (f_diffuse < .0f) ? Vect{0, 0, 0} : color * f_diffuse * this->k_diffuse;
  // Specular
  float f_specular = pow(std::max(0.f, L * !out_ray_dir), this->hardness);
  Vect I_specular = light.color * f_specular * this->k_specular;
  // return I_specular;
  return I_ambient + I_diffuse + I_specular;
  // return color;
}

void Material::set_texture(Texture *p_t) { this->p_texture = p_t; }