#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include "Model.h"
#include "utils/Shader.h"

class GameObject {
public:
    GameObject(glm::vec3 pos, Model model, glm::vec3 velocity = glm::vec3(0.0f));
    void Draw(Shader &shader);
private:
    glm::vec3   Position, Velocity;
    GLfloat     Rotation, Scale;
    Model mModel;
};


#endif