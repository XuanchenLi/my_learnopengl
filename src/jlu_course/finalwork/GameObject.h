#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include "Model.h"
#include "utils/Shader.h"

class GameObject {
public:
    GameObject(glm::vec3 pos, Model model, glm::vec3 velocity = glm::vec3(0.0f));
    void Draw(Shader &shader);
    glm::vec3 GetPosition() {return Position;}
    void SetPosition(glm::vec3&pos) {Position = pos;}
    GLfloat GetRotation() {return Rotation;}
    void SetRotation(GLfloat r) {Rotation = r;}
    
    glm::vec3   Position, Velocity;
    GLfloat     Rotation, Scale;
    Model mModel;
};


#endif