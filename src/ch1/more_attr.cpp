#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"


float vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};


const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main ()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0f);\n"
    "   ourColor = aColor;\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main ()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);"
    "}\n";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window ,true);
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    //initialize window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout<<"Fail" <<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //initialize glad 
    if (!gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress )) {
        std::cout<<"GLAD Fail"<<std::endl;
        glfwTerminate();
        return -1;
    }

    //set size of viewport
    glViewport(0, 0, 800, 600);

    //bind window size callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //initialize our Shader class
    Shader ourShader("Shader/shader.vs", "Shader/shader.fs");

    //initialize VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //initialize VBO;
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //after register VBO to VAO by glVertexAttribPointer, buffer can be unbind safely
    glBindBuffer(GL_VERTEX_ARRAY, 0);


    //render loop
    while (!glfwWindowShouldClose(window)) {
        //process input
        processInput(window);
        //render command
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // state set function
        glClear(GL_COLOR_BUFFER_BIT);  // state use function
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //double buffer
        glfwSwapBuffers(window);
        //event handle
        glfwPollEvents();
    }

    //optional delete source
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    return 0;
}
