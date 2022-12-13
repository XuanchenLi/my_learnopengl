#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include <glm/glm.hpp>
#include "Light.h"

class PointLight : public Light {
public:
    PointLight(){}
    PointLight(glm::vec3 pos, GLfloat con, GLfloat lin, GLfloat quad, glm::vec3 am, glm::vec3 di, glm::vec3 sp)
        : Light(am, di, sp), position(pos), constant(con), linear(lin), quadratic(quad) {}

    glm::vec3 position;
    GLfloat constant, linear, quadratic;
};

#endif