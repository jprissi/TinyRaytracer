#include "vector.h"
#include "light.h"



// class Light {
//     public:
//         Vect pos;
//         Vect color = {255, 255, 255};
//         Light(Vect position): pos(position) {};
//         // Light(): pos(Vect{0,0,0}) {};
//         float k_ambient = 0.2f;
// };
Light::Light(Vect position): pos(position), color({255,255,255}) {
    //ambient light is more specific to scene than light, will be moved elsewhere soon
    k_ambient = 0.1f;
    k_diffuse = 0.4f;
}