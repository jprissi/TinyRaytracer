#ifndef OBJ_PLANE
#define OBJ_PLANE

#include "object.h"
#include "triangle.h"

class Plane : public Object {
public:
  vect3 center;
  Triangle tri1, tri2;
  float width, height;

  Plane(vect3 center, float width, float height)
      : center(center), width(width), height(height) {

    vect3 p0 = center + width / 2 + height / 2;
    vect3 p1, p2;
    vect3 p3 = center - (p1 - center);

    tri1 = Triangle(p0, p1, p2);
    tri2 = Triangle(p2, p3, p0)
  }

  bool is_hit(const vect3 &incoming_ray_origin,
              const vect3 &incoming_ray_dir) const;
  bool is_hit(const vect3 &incoming_ray_origin, const vect3 &incoming_ray_dir,
              vect3 &outgoing_ray_origin, vect3 &outgoing_ray_dir,
              float &hit_distance,
              // vect3& hit_color
              Material &hit_material, vect3 &hit_color, float ray_energy,
              Light light_source) const;
  void set_pos(const vect3 &new_pos) {
    std::cout << "WARNING : Changing position of triangle not supported yet"
              << std::endl;
    Object::set_pos(new_pos);
  }
};

#endif