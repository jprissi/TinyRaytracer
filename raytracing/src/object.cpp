#include "object.h"
#include <stdio.h>

Vect Object::compute_color(Material mat, Light light, Vect hit_point, Vect hit_normal, Vect out_ray_dir) const
{
  // Vect color{0, 0, 0};
  Vect L = !(light.pos - hit_point);
  //Ambient light
  Vect I_ambient = mat.color * light.k_ambient;
  //Diffuse
  float f_diffuse = L * hit_normal;
  Vect I_diffuse = (f_diffuse < .0f) ? Vect{0, 0, 0} : mat.color * f_diffuse * mat.k_diffuse;
  //Specular
  float f_specular = pow(std::max(0.f, L * !out_ray_dir), mat.hardness);
  Vect I_specular = light.color * f_specular * mat.k_specular;
  //return I_specular;
  return I_ambient + I_diffuse + I_specular;
}

bool Triangle::is_hit(
    const Vect &incoming_ray_origin,
    const Vect &incoming_ray_dir) const
{
  //for now, no shadow
  return false;
  //TODO This is basically a copy paste of the next function, reformat it!
  if (n * incoming_ray_dir >= 0)
    return false;

  float hit_distance = 0.0f;

  const float &pox{p0.x};
  const float &poy{p0.y};
  const float &poz{p0.z};
  const float &ux{u.x};
  const float &uy{u.y};
  const float &uz{u.z};
  const float &vx{v.x};
  const float &vy{v.y};
  const float &vz{v.z};
  const float &rx{incoming_ray_dir.x};
  const float &ry{incoming_ray_dir.y};
  const float &rz{incoming_ray_dir.z};
  const float &ox{incoming_ray_origin.x};
  const float &oy{incoming_ray_origin.y};
  const float &oz{incoming_ray_origin.z};

  const float u_fact = -(ox * ry * vz - ox * rz * vy - oy * rx * vz + oy * rz * vx + oz * rx * vy - oz * ry * vx - pox * ry * vz + pox * rz * vy + poy * rx * vz - poy * rz * vx - poz * rx * vy + poz * ry * vx) / (rx * uy * vz - rx * uz * vy - ry * ux * vz + ry * uz * vx + rz * ux * vy - rz * uy * vx);
  const float v_fact = (ox * ry * uz - ox * rz * uy - oy * rx * uz + oy * rz * ux + oz * rx * uy - oz * ry * ux - pox * ry * uz + pox * rz * uy + poy * rx * uz - poy * rz * ux - poz * rx * uy + poz * ry * ux) / (rx * uy * vz - rx * uz * vy - ry * ux * vz + ry * uz * vx + rz * ux * vy - rz * uy * vx);
  const float ray_fact = (-(ox - pox) * (uy * vz - uz * vy) + (oy - poy) * (ux * vz - uz * vx) - (oz - poz) * (ux * vy - uy * vx)) / (rx * uy * vz - rx * uz * vy - ry * ux * vz + ry * uz * vx + rz * ux * vy - rz * uy * vx);

  if (u_fact < 0 or u_fact > 1 or
      v_fact < 0 or v_fact > 1 or
      u_fact + v_fact > 1 or
      ray_fact < 0)
  {
    return false;
  }

  hit_distance = (incoming_ray_dir * ray_fact).length();

  if (hit_distance < 1e-3)
    return false;
  return true;
}

bool Triangle::is_hit(
    const Vect &incoming_ray_origin,
    const Vect &incoming_ray_dir,
    Vect &outgoing_ray_origin,
    Vect &outgoing_ray_dir,
    float &hit_distance,
    //Vect& hit_color
    Material &hit_material,
    Vect &hit_color,
    float ray_energy,
    Light light_source) const
{
  if (n * incoming_ray_dir >= 0)
    return false;

  //Proceed
  // float pox, poy, poz;
  // float ox, oy, oz;
  // float ux, uy, uz;
  // float vx, vy, vz;
  // float rx, ry, rz;

  // pox, poy, poz = p0.x, p0.y, p0.z;
  // ox, oy, oz = incoming_ray_origin.x, incoming_ray_origin.y, incoming_ray_origin.z;
  // ux, uy, uz = u.x, u.y, u.z;
  // vx, vy, vz = v.x, v.y, v.z;
  // rx, ry, rz = incoming_ray_dir.x, incoming_ray_dir.y, incoming_ray_dir.z;

  const float &pox{p0.x};
  const float &poy{p0.y};
  const float &poz{p0.z};
  const float &ux{u.x};
  const float &uy{u.y};
  const float &uz{u.z};
  const float &vx{v.x};
  const float &vy{v.y};
  const float &vz{v.z};
  const float &rx{incoming_ray_dir.x};
  const float &ry{incoming_ray_dir.y};
  const float &rz{incoming_ray_dir.z};
  const float &ox{incoming_ray_origin.x};
  const float &oy{incoming_ray_origin.y};
  const float &oz{incoming_ray_origin.z};

  const float u_fact = -(ox * ry * vz - ox * rz * vy - oy * rx * vz + oy * rz * vx + oz * rx * vy - oz * ry * vx - pox * ry * vz + pox * rz * vy + poy * rx * vz - poy * rz * vx - poz * rx * vy + poz * ry * vx) / (rx * uy * vz - rx * uz * vy - ry * ux * vz + ry * uz * vx + rz * ux * vy - rz * uy * vx);
  const float v_fact = (ox * ry * uz - ox * rz * uy - oy * rx * uz + oy * rz * ux + oz * rx * uy - oz * ry * ux - pox * ry * uz + pox * rz * uy + poy * rx * uz - poy * rz * ux - poz * rx * uy + poz * ry * ux) / (rx * uy * vz - rx * uz * vy - ry * ux * vz + ry * uz * vx + rz * ux * vy - rz * uy * vx);
  const float ray_fact = (-(ox - pox) * (uy * vz - uz * vy) + (oy - poy) * (ux * vz - uz * vx) - (oz - poz) * (ux * vy - uy * vx)) / (rx * uy * vz - rx * uz * vy - ry * ux * vz + ry * uz * vx + rz * ux * vy - rz * uy * vx);

  if (u_fact < 0 or u_fact > 1 or
      v_fact < 0 or v_fact > 1 or
      u_fact + v_fact > 1 or
      ray_fact < 0)
  {
    return false;
  }

  hit_distance = (incoming_ray_dir * ray_fact).length();

  if (hit_distance < 1e-3)
    return false;

  outgoing_ray_origin = p0 + u * u_fact + v * v_fact;
  outgoing_ray_dir = !Vect{incoming_ray_dir - !n * (incoming_ray_dir * !n) * 2};

  hit_material = material;
  hit_color = material.color * (1 - material.reflectivity) * ray_energy;
  return true;
}