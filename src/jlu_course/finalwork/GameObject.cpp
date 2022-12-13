#include "GameObject.h"


GameObject::GameObject(glm::vec3 pos, Model model, glm::vec3 velocity) 
    : Position(pos), Velocity(velocity), Rotation(0.0f), Scale(1.0f), mModel(model){ }

void GameObject::Draw(Shader &shader)
{
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, Position); 
    model = glm::rotate(model, glm::radians(Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(Scale));	
    shader.setMat4("model", model);
    mModel.Draw(shader);
}