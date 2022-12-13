#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils/Shader.h"



typedef struct Particle
{
    GLfloat LifeTime;
    GLfloat Scale;
    GLfloat Rotate;
    glm::vec3 Position;
    glm::vec3 Velocity;
    Particle() 
        : Position(0.0f), Velocity(0.0f), LifeTime(0.0f), Scale(1.0f), Rotate(0.0f) { }
    bool isDead() {
        return Position.y <= 0.0f;
    }
}Particle;


class ParticleManager {
public:
    ParticleManager(Shader shader, GLuint vao, GLuint num = 50)
        :particleNum(num), shader(shader), VAO(vao){};
    ~ParticleManager() {
        glDeleteBuffers(1, &instancedVBO);
        delete[] modelMatrices;
    }
    bool init();
    void update(GLfloat deltaTimeMillis, GLfloat currentFrame);
    void draw();

private:
    const GLuint particleNum;
    std::vector<Particle> particles;
    Shader shader;
    GLuint VAO;
    GLuint instancedVBO;
    glm::mat4* modelMatrices;
    GLint getFirstDeadParticle();
    void respawnParticle(Particle& particle);
};

#endif