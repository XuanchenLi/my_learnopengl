#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H
#include <glm/glm.hpp>
#include "Light.h"

class SpotLight : public Light {
public:
    SpotLight(){}
    SpotLight(glm::vec3 pos, glm::vec3 dir, 
        GLfloat cut, GLfloat outerCut,
        GLfloat con, GLfloat lin, GLfloat quad, glm::vec3 am, glm::vec3 di, glm::vec3 sp)
        : Light(am, di, sp), direction(dir), position(pos), 
            cutOff(cut), outerCutOff(outerCut),
            constant(con), linear(lin), quadratic(quad) {}

    glm::vec3 position;
    glm::vec3 direction;
    GLfloat cutOff, outerCutOff;
    GLfloat constant, linear, quadratic;
};

#endif