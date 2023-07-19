#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"

class Light {
    public:
        vect3 pos;
        vect3 color;
	float k_ambient;
        float k_diffuse;
	
        Light(vect3);
        Light();
};

#endif
