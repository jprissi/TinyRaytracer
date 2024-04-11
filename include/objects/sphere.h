#ifndef OBJ_SPHERE
#define OBJ_SPHERE

#include "objects/object.h"

class Sphere : public Object
{

public:
  Sphere(const vect3 &position, const float &radius) : radius(radius) { setPos(pos); };
  Sphere(const float &radius) : Sphere({0, 0, 0}, radius){};

  float getRadius() const { return radius; }
  void setPos(const vect3 &new_pos) { pos = new_pos; };

  bool is_hit(const vect3 &incoming_ray_origin, const vect3 &incoming_ray_dir) const override;
  bool is_hit(const vect3 &incoming_ray_origin, const vect3 &incoming_ray_dir,
              vect3 &outgoing_ray_origin, vect3 &outgoing_ray_dir,
              float &hit_distance, Material &hit_material, vect3 &hit_color,
              float ray_energy, Light light_source) const override;

  vect2 map_3D_to_UV_coordinates(
      vect3 const &hit_point) const override; // Overriding from object

  Color computeColor(Material mat, Light light, vect3 hit_point,
                      vect3 hit_normal, vect3 out_ray_dir,
                      vect3 object_center) const override;

private:
  float radius;
};

#endif