#include "ParticleManager.h"
#include <glm/gtx/string_cast.hpp>


bool ParticleManager::init() {
    particles.clear();
    for (int i = 0; i < particleNum; ++i) {
        particles.push_back(Particle());
        //respawnParticle(particles[i]);
        particles[i].Scale = 0.0;
    }
    
    modelMatrices = new glm::mat4[particleNum*12];
    glGenBuffers(1, &instancedVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
    glBufferData(GL_ARRAY_BUFFER, 12 * particleNum * sizeof(glm::mat4), &modelMatrices[0], GL_DYNAMIC_DRAW);
    glBindVertexArray(VAO);
    GLsizei vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(2); 
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(4); 
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);
    
    return true;
}

GLint ParticleManager::getFirstDeadParticle() {
    static GLuint lastDeadIndex = 0;
    for (GLuint i = lastDeadIndex; i < this->particleNum; ++i) {
        if (this->particles[i].isDead()) {
            lastDeadIndex = i;
            return i;
        }
    }
    for (GLuint i = 0; i < lastDeadIndex; ++i){
        if (this->particles[i].isDead()){
            lastDeadIndex = i;
            return i;
        }
    }
    lastDeadIndex = 0;
    return -1;
}

void ParticleManager::respawnParticle(Particle& particle) {
    particle.LifeTime = 1.0f;
    particle.Position = glm::vec3(((rand() % 80) - 40), 25.0f, ((rand() % 80) - 40));
    //std::cout<<glm::to_string(particle.Position)<<std::endl;
    particle.Velocity = glm::vec3(0.0f, -(rand() % 2), 0.0f);
    particle.Scale = (rand() % 10 / 10.0) * 0.05;
}

void ParticleManager::update(GLfloat deltaTimeMillis, GLfloat currentFrame) {
    GLint index = getFirstDeadParticle();
    GLuint cnt = 0;
    while(index != -1) {
        cnt++;
        if (cnt >= 20)
            break;
        respawnParticle(particles[index]);
        index = getFirstDeadParticle();
    }
    glm::vec3 acceleration = glm::vec3(0.5f, -1.0f, 0.0f);
    for (int i = 0; i < this->particleNum; ++i) {
        Particle& p = particles[i];
        p.LifeTime -= deltaTimeMillis;
        if (!p.isDead()) {
            p.Position += p.Velocity * deltaTimeMillis;
            //p.Position = glm::vec3(1.0f);
            p.Velocity += acceleration * deltaTimeMillis;
            p.Rotate = currentFrame;
        }
        
        for (int j = 0; j < 6; ++j) {
            glm::mat4 model = glm::mat4(1.0);
            model = glm::translate(model, p.Position);
            model = glm::rotate(model, glm::radians(p.Rotate), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(60.0f * j), glm::vec3(0.0, 0.0, 1.0f));
            model = glm::scale(model, glm::vec3(p.Scale));
            modelMatrices[(12 * i) + (2 * j)] = model;
            model = glm::scale(model, glm::vec3(1.0f, -1.0f, 1.0f));
            modelMatrices[(12 * i) + (2 * j) + 1] = model;
        }
        
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
    void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, modelMatrices, sizeof(glm::mat4) * particleNum * 12);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    
}

void ParticleManager::draw() {
    this->shader.use();
    glBindVertexArray(this->VAO);
    /*
    for (Particle particle : this->particles)
    {
        if (!particle.isDead())
        {
            for (int i = 0; i < 6; ++i) {
                    glm::mat4 model = glm::mat4(1.0);
                    model = glm::translate(model, particle.Position);
                    model = glm::rotate(model, glm::radians(particle.Rotate), glm::vec3(0.0f, 0.0f, 1.0f));
                    model = glm::rotate(model, glm::radians(60.0f * i), glm::vec3(0.0, 0.0, 1.0f));
                    model = glm::scale(model, glm::vec3(particle.Scale));
                    this->shader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLE_FAN, 0, 12);
                    model = glm::scale(model, glm::vec3(1.0f, -1.0f, 1.0f));
                    this->shader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLE_FAN, 0, 12);
            }
        }
    }
    */
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 12, 12 * particleNum);
    glBindVertexArray(0);
}