#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TextureLoader {
public:
    static unsigned int loadTexture(const char* path);
    static unsigned int loadHDRI(const char* path);
    static unsigned int loadCubemap(std::vector<std::string> faces);

};


#endif