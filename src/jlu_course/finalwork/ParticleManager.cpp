#include "ParticleManager.h"


bool ParticleManager::init() {
    particles.clear();
    for (int i = 0; i < particleNum; ++i) {
        particles.push_back(Particle());
    }
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
    particle.Position = glm::vec3(((rand() % 80) - 40), 20.0f, ((rand() % 80) - 40));
    particle.Velocity = glm::vec3(0.0f, rand() % 5, 0.0f);
    particle.Scale = rand() % 10 / 10.0;
}

void ParticleManager::update(GLfloat deltaTimeMillis, GLfloat currentFrame) {
    GLint index = getFirstDeadParticle();
    while(index != -1) {
        respawnParticle(particles[index]);
        index = getFirstDeadParticle();
    }
    glm::vec3 acceleration = glm::vec3(4.0f, -9.81f, 0.0f);
    for (int i = 0; i < this->particleNum; ++i) {
        Particle& p = particles[i];
        p.LifeTime -= deltaTimeMillis;
        if (!p.isDead()) {
            p.Position += p.Velocity * deltaTimeMillis;
            p.Velocity += acceleration * deltaTimeMillis;
            p.Rotate = currentFrame;
        }
    }
}

void ParticleManager::draw() {
    this->shader.use();
    for (Particle particle : this->particles)
    {
        if (!particle.isDead())
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, particle.Position);
            model = glm::rotate(model, glm::radians(particle.Rotate), glm::vec3(0.0f, 0.0f, 1.0f));
            this->shader.setMat4("model", model);
            //this->texture.Bind();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
}