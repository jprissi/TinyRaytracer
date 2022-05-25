#ifndef MAT_H
#define MAT_H

#include "vector.h"

class Material
{
    /**
   * Defines a basic material
   */
public:
    Material() : color(0, 0, 0), reflectivity(0) {}
    Vect color;
    float reflectivity;
    float k_diffuse = 0.4f;
    float k_specular = 0.4f;
    float hardness = 100.0f;
};

#endif