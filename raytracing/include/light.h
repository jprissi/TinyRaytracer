#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"

class Light {
    public:
        Vect pos;
        Vect color;
        Light(Vect);
        // Light();
        float k_ambient;
        float k_diffuse;
};

#endif