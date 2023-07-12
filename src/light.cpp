#include "vector.h"
#include "light.h"

Light::Light(Vect position): pos(position), color({0,0,0}), k_ambient(0), k_diffuse(0) {
    //ambient light is more specific to scene than light, will be moved elsewhere soon
    color = {255,255,255};
    k_ambient = 0.1f;
    k_diffuse = 0.4f;
}
//Calling already existing extensive constructor
Light::Light(): Light::Light(Vect{0,0,0}) {}
