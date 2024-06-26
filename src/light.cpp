#include "vector.h"
#include "light.h"

Light::Light(vect3 position): pos(position), color({0,0,0}), k_ambient(0), k_diffuse(0) {
    //ambient light is more specific to scene than light, will be moved elsewhere soon
    color = {255,255,255};
    k_ambient = 0.8f;
    k_diffuse = 0.6f;
}
//Calling already existing extensive constructor
Light::Light(): Light::Light(vect3{0,0,0}) {}
