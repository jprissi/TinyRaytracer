#ifndef OBJ_TRIANGLE
#define OBJ_TRIANGLE

#include "object.h"

class Triangle : public Object {
public:
  Vect p0, p1, p2, u, v, n;
  Triangle(const Vect &p0, const Vect &p1, const Vect &p2)
      : p0(p0), p1(p1), p2(p2) {
    u = p1 - p0;
    v = p2 - p0;
    n = v ^ u;
    pos = (p0 + p1 + p2) * (1.0f / 3.0f);
  }
  bool is_hit(const Vect &incoming_ray_origin,
              const Vect &incoming_ray_dir) const;
  bool is_hit(const Vect &incoming_ray_origin, const Vect &incoming_ray_dir,
              Vect &outgoing_ray_origin, Vect &outgoing_ray_dir,
              float &hit_distance,
              // Vect& hit_color
              Material &hit_material, Vect &hit_color, float ray_energy,
              Light light_source) const;
  void set_pos(const Vect &new_pos) {
    std::cout << "WARNING : Changing position of triangle not supported yet"
              << std::endl;
    Object::set_pos(new_pos);
  }
};

#endif