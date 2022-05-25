#include "stdio.h"
#include "object.h"
#include <vector>
#include "light.h"
#include "camera.h"

class Scene {
    public:
        // Container for the objects of the scene
        std::vector<Object*> scene_objects;
        Light lightSource;
        Camera camera;
        void addObject(Object* obj, Vect color, float reflectivity);
        //void setCamera(Camera c) : camera(c) {};
        void init();
};