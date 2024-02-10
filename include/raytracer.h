#ifndef TRACER_H
#define TRACER_H

#include <stdio.h>
#include <vector>
#include "scene.h"
#include <random>
#include <cmath>
#include "ray.hpp"

class Raytracer
{
public:
    Raytracer(Scene &scene, Camera const &camera);
    void compute(std::vector<unsigned char> &img);

private:
    Ray ray;
    Scene scene;
    Camera camera;
    void export_color(vect3 color, std::vector<unsigned char> &img);
    bool propagate_ray(vect3 *p_outgoing_ray_origin, vect3 *p_outgoing_ray_dir,
                       vect3 *p_color);
    float random_offset();
    float clip(float component);
    vect3 sky_color(vect3 direction);
    vect3 ground_color(vect3 direction, vect3 origin);
};

#endif