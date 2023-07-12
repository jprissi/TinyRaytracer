#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"

class Light {
    public:
        Vect pos;
        Vect color;
	float k_ambient;
        float k_diffuse;
	
        Light(Vect);
        Light();
};

#endif
