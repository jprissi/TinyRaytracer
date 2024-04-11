#include "objects/sphere.h"
#include "material/texture.h"
#include <stdio.h>
#include <cassert>

// This is related to object rather than texture
vect2 Sphere::map_3D_to_UV_coordinates(
    vect3 const &hit_point) const
{
    // For Sphere:

    vect3 d = !(hit_point - this->pos);
    float u = 0.5 + std::atan2(d.z, d.x) / (2 * M_PI);
    float v = 0.5 + std::asin(d.y) / M_PI;

    ImageTexture *p_texture = (ImageTexture *)material.get_texture();

    uint xsize, ysize;
    p_texture->getTextureSize(xsize, ysize);
    // assert(xsize > 0);
    // assert(ysize > 0);

    unsigned x = std::round(u * (xsize - 1)) + 0.5;
    x = std::min(x, xsize - 1);
    unsigned y = std::round(v * (ysize - 1)) + 0.5;
    y = std::min(y, ysize - 1);

    return vect2{x, y};
    // return p_texture->getTexturePixel(x, y);
}

Color Sphere::computeColor(Material mat, Light light, vect3 hit_point,
                           vect3 hit_normal, vect3 out_ray_dir,
                           vect3 object_center) const
{
    vect2 UV_coordinates = this->map_3D_to_UV_coordinates(hit_point);
    // std::cout << UV_coordinates << std::endl;

    Color color = this->material.get_texture()->getColor(UV_coordinates, this->pos);
    
    return color;
}

bool Sphere::is_hit(const vect3 &incoming_ray_origin, const vect3 &incoming_ray_dir,
                    vect3 &outgoing_ray_origin, vect3 &outgoing_ray_dir,
                    float &hit_distance, Material &hit_material, Color &hit_color,
                    float ray_energy, Light light_source) const
{

    /**
     * These geometric construcs can be complex to explain without a drawing.
     * See reference
     */

    vect3 p = pos - incoming_ray_origin;
    // vect3 p = incoming_ray_origin - pos;
    float b = p * incoming_ray_dir;
    float hit_limit = sqrt(p * p - pow(radius, 2));
    float s_squared = p * p - pow(b, 2);
    float t_squared = pow(radius, 2) - s_squared;

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
    hit_color = computeColor(material, light_source, outgoing_ray_origin,
                             normal, outgoing_ray_dir, pos) *
                (1 - material.reflectivity) * ray_energy;
    return true;
}

bool Sphere::is_hit(const vect3 &incoming_ray_origin,
                    const vect3 &incoming_ray_dir) const
{
    /**
     * These geometric construcs can be complex to explain without a drawing.
     * See reference
     */
    vect3 p = pos - incoming_ray_origin;
    float b = p * incoming_ray_dir;
    float hit_limit = sqrt(p * p - pow(radius, 2));

    return (b < hit_limit) ? false : true;
}