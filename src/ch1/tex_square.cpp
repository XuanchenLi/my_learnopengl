#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"


float vertices[] = {
//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

float ratio = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window ,true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        ratio += 0.001;
        if (ratio > 1.0f)
            ratio = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        ratio -= 0.001;
        if (ratio < 0.0f)
            ratio = 0.0f;
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

    Shader ourShader("Shader/tex_shader.vs", "Shader/tex_shader.fs");
    

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //initialize EBO/IBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //after register VBO to VAO by glVertexAttribPointer, buffer can be unbind safely
    glBindBuffer(GL_VERTEX_ARRAY, 0);
    //DO NOT UNBIND GL_ELEMENT_ARRAY_BUFFER CAUSE IT WILL BE SAVED BY VAO
    // !! glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); !!

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // 不要为纹理放大过滤设置为多级渐远纹理过滤选项之一，多级渐远纹理过滤只能用于纹理缩小
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("Texture/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // 不要为纹理放大过滤设置为多级渐远纹理过滤选项之一，多级渐远纹理过滤只能用于纹理缩小
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("Texture/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //设置unifrom
    ourShader.use();
    ourShader.setInt("ourTexture1", 0);
    ourShader.setInt("ourTexture2", 1);
    //render loop
    while (!glfwWindowShouldClose(window)) {
        //process input
        processInput(window);
        //render command
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // state set function
        glClear(GL_COLOR_BUFFER_BIT);  // state use function
        ourShader.use();
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(VAO);
        //绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        ourShader.setFloat("ratio", ratio);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        

        //double buffer
        glfwSwapBuffers(window);
        //event handle
        glfwPollEvents();
    }

    //optional delete source
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
    return 0;
}
