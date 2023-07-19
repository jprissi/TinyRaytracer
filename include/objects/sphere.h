#ifndef OBJ_SPHERE
#define OBJ_SPHERE

#include "objects/object.h"

class Sphere : public Object {

public:
  Sphere(const vect3 &position, const float &radius)
      : r(radius), pos(position){};
  Sphere(const float &radius) : Sphere({0, 0, 0}, radius){};
  float get_radius() { return r; }
  void set_pos(const vect3 &new_pos) { pos = new_pos; };
  bool is_hit(const vect3 &incoming_ray_origin,
              const vect3 &incoming_ray_dir) const {
    /**
     * These geometric construcs can be complex to explain without a drawing.
     * See reference
     */
    vect3 p = pos - incoming_ray_origin;
    float b = p * incoming_ray_dir;
    float hit_limit = sqrt(p * p - pow(r, 2));

    return (b < hit_limit) ? false : true;
  }
  bool is_hit(const vect3 &incoming_ray_origin, const vect3 &incoming_ray_dir,
              vect3 &outgoing_ray_origin, vect3 &outgoing_ray_dir,
              float &hit_distance, Material &hit_material, vect3 &hit_color,
              float ray_energy, Light light_source) const {

    /**
     * These geometric construcs can be complex to explain without a drawing.
     * See reference
     */

    vect3 p = pos - incoming_ray_origin;
    // vect3 p = incoming_ray_origin - pos;
    float b = p * incoming_ray_dir;
    float hit_limit = sqrt(p * p - pow(r, 2));
    float s_squared = p * p - pow(b, 2);
    float t_squared = pow(r, 2) - s_squared;

    if (b < hit_limit)
      return false;

    // Compute distance at hit
    float t = sqrt(t_squared);
    hit_distance = b - t;

    if (hit_distance >= hit_limit || hit_distance < 1e-3)
      return false;
    // Compute normal
    vect3 normal = !(incoming_ray_dir * hit_distance - p);

    outgoing_ray_origin = incoming_ray_origin + incoming_ray_dir * hit_distance;
    outgoing_ray_dir =
        !vect3{incoming_ray_dir - (normal * (incoming_ray_dir * normal) * 2)};

    // if (p*incoming_ray_dir >= hit_limit) return false; //there's a hit
    hit_material = material;
    hit_color = compute_color(material, light_source, outgoing_ray_origin,
                              normal, outgoing_ray_dir, pos) *
                (1 - material.reflectivity) * ray_energy;
    return true;
  }

private:
  float r;
  vect3 pos;
};

#endif