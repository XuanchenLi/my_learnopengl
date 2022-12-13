#ifndef LIGHT_H
#define LIGHT_H
#include <glm/glm.hpp>

class Light {
public:
    Light(){}
    Light(glm::vec3 a, glm::vec3 d, glm::vec3 s)
        : ambient(a), diffuse(d), specular(s){}

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

#endif