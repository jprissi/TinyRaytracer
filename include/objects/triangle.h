#ifndef OBJ_TRIANGLE
#define OBJ_TRIANGLE

#include "object.h"

class Triangle : public Object {
public:
  vect3 p0, p1, p2, u, v, n;
  Triangle(const vect3 &p0, const vect3 &p1, const vect3 &p2)
      : p0(p0), p1(p1), p2(p2) {
    u = p1 - p0;
    v = p2 - p0;
    n = v ^ u;
    pos = (p0 + p1 + p2) * (1.0f / 3.0f);
  }
  bool is_hit(const vect3 &incoming_ray_origin,
              const vect3 &incoming_ray_dir) const;
  bool is_hit(const vect3 &incoming_ray_origin, const vect3 &incoming_ray_dir,
              vect3 &outgoing_ray_origin, vect3 &outgoing_ray_dir,
              float &hit_distance,
              // vect3& hit_color
              Material &hit_material, vect3 &hit_color, float ray_energy,
              Light light_source) const;
  void setPos(const vect3 &new_pos) {
    std::cout << "WARNING : Changing position of triangle not supported yet"
              << std::endl;
    Object::setPos(new_pos);
  }
};

#endif