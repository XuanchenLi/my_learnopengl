#ifndef DIRLIGHT_H
#define DIRLIGHT_H
#include <glm/glm.hpp>
#include "Light.h"

class DirLight : public Light {
public:
    DirLight(){}
    DirLight(glm::vec3 dir, glm::vec3 am, glm::vec3 di, glm::vec3 sp)
        : Light(am, di, sp), direction(dir) {}
    
    glm::vec3 direction;
};

#endif