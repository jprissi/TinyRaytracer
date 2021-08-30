#include "vector.h"
#include <iostream>

class Material {
  /**
   * Defines a basic material
   */
    public:
        Material() : color(0, 0, 0), reflectivity(0) {}
        Vect color;
        float reflectivity;
};

class Object  {
    /**
     * Define a generic object class for different meshes that could appear on screen
     */

    public:
        Object() : material() {}

        virtual bool is_hit(
            const Vect& incoming_ray_origin,
            const Vect& incoming_ray_dir,
            Vect& outgoing_ray_origin,
            Vect& outgoing_ray_dir,
            float& hit_distance,
            //Vect& hit_color
            Material& hit_material
        ) const = 0;

        void set_color(const Vect& v) {
            material.color = v;
        }
        void set_reflectivity(const float& f) {
            material.reflectivity = f;
        }
        Material material;
        Vect pos; 
};

class Triangle: public Object {
    public:
        Triangle(
            const Vect& p0,
            const Vect& p1,
            const Vect& p2
            ) : p0(p0), p1(p1), p2(p2) {
                u = p1 - p0;
                v = p2 - p0;
                n = v^u;
            }

        bool is_hit(
            const Vect& incoming_ray_origin,
            const Vect& incoming_ray_dir,
            Vect& outgoing_ray_origin,
            Vect& outgoing_ray_dir,
            float& hit_distance,
            //Vect& hit_color
            Material& hit_material
        ) const {
            if (n*incoming_ray_dir >= 0) return false;

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

            const float& pox{p0.x};
            const float& poy{p0.y};
            const float& poz{p0.z};
            const float& ux{u.x};
            const float& uy{u.y};
            const float& uz{u.z};
            const float& vx{v.x};
            const float& vy{v.y};
            const float& vz{v.z};
            const float& rx{incoming_ray_dir.x};
            const float& ry{incoming_ray_dir.y};
            const float& rz{incoming_ray_dir.z};
            const float& ox{incoming_ray_origin.x};
            const float& oy{incoming_ray_origin.y};
            const float& oz{incoming_ray_origin.z};

            // cout << incoming_ray_dir.length()<<"; "<< incoming_ray_origin.length() << "\n";
            // cout << (-(ox - pox)*(uy*vz - uz*vy) + (oy - poy)*(ux*vz - uz*vx) - (oz - poz)*(ux*vy - uy*vx)) << "\n";
            // cout << (rx*uy*vz - rx*uz*vy - ry*ux*vz + ry*uz*vx + rz*ux*vy - rz*uy*vx) << "\n";
            //const float ray_fact = -(ox*(uy*vz - uz*vy) - oy*(ux*vz - uz*vx) + oz*(ux*vy - uy*vx) - pox*( uy*vz + uz*vy) + poy*(ux*vz - uz*vx) - poz*(ux*vy + uy*vx)) / (rx*uy*vz - rx*uz*vy - ry*ux*vz + ry*uz*vx + rz*ux*vy - rz*uy*vx);
            const float u_fact = -(ox*ry*vz - ox*rz*vy - oy*rx*vz + oy*rz*vx + oz*rx*vy - oz*ry*vx - pox*ry*vz + pox*rz*vy + poy*rx*vz - poy*rz*vx - poz*rx*vy + poz*ry*vx) / (rx*uy*vz - rx*uz*vy - ry*ux*vz + ry*uz*vx + rz*ux*vy - rz*uy*vx);
            const float v_fact = (ox*ry*uz - ox*rz*uy - oy*rx*uz + oy*rz*ux + oz*rx*uy - oz*ry*ux - pox*ry*uz + pox*rz*uy + poy*rx*uz - poy*rz*ux - poz*rx*uy + poz*ry*ux) / (rx*uy*vz - rx*uz*vy - ry*ux*vz + ry*uz*vx + rz*ux*vy - rz*uy*vx);
            // const float u_fact = (-(ox - pox)*(ry*vz - rz*vy) + (oy - poy)*(rx*vz - rz*vx) - (oz - poz)*(rx*vy - ry*vx))/(rx*uy*vz - rx*uz*vy - ry*ux*vz + ry*uz*vx + rz*ux*vy - rz*uy*vx);
		    // const float v_fact = ((ox - pox)*(ry*uz - rz*uy) - (oy - poy)*(rx*uz - rz*ux) + (oz - poz)*(rx*uy - ry*ux))/(rx*uy*vz - rx*uz*vy - ry*ux*vz + ry*uz*vx + rz*ux*vy - rz*uy*vx);
		    const float ray_fact = (-(ox - pox)*(uy*vz - uz*vy) + (oy - poy)*(ux*vz - uz*vx) - (oz - poz)*(ux*vy - uy*vx))/(rx*uy*vz - rx*uz*vy - ry*ux*vz + ry*uz*vx + rz*ux*vy - rz*uy*vx);


            if( u_fact < 0 or u_fact > 1 or 
                v_fact < 0 or v_fact > 1 or 
                u_fact + v_fact > 1 or
                ray_fact < 0) {
                return false;
            }

            // cout << ray_fact << "; " << incoming_ray_dir.length() << "\n";
            hit_distance = (incoming_ray_dir*ray_fact).length();
            // std::cout << hit_distance << "\n";
            if (hit_distance < 1e-3) return false;

            outgoing_ray_origin = p0 + u*u_fact + v*v_fact;
            outgoing_ray_dir = !Vect{incoming_ray_dir - !n*(incoming_ray_dir*!n)*2};

            //hit_color = material.color;
            hit_material = material;

            return true;

        }

        Vect p0, p1, p2, u, v, n;
};
