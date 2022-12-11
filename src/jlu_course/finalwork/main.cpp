#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <map>
#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"
#include "ParticleManager.h"
#include "utils/Shader.h"
#include "utils/Camera.h"


// settings
unsigned int SCR_WIDTH = 1920;
unsigned int SCR_HEIGHT = 1080;
GLFWwindow* window;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

bool surround = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//雪花顶点数据
float snowVertices[] = {
    //----顶点坐标---
    0.2, 0.2, 0.0,
    1.2, 0.2, 0.0,
    2.2, 1.2, 0.0,
    2.5, 1.2, 0.0,
    1.5, 0.2, 0.0,
    2.5, 0.2, 0.0,
    3.2, 0.9, 0.0,
    3.5, 0.9, 0.0,
    2.8, 0.2, 0.0,
    3.8, 0.2, 0.0,
    4.1, 0.0, 0.0,
    0.0, 0.0, 0.0,
};


int initWindow() {
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //设置居中
    int monitorCount;
    GLFWmonitor** pMonitor = glfwGetMonitors(&monitorCount);
    printf("%d\n", monitorCount);
    const GLFWvidmode* mode1 = glfwGetVideoMode(*pMonitor);	
    glfwSetWindowPos(window, (mode1->width - SCR_WIDTH) / 2, (mode1->height - SCR_HEIGHT) / 2);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return 1;
}



int main() {

    if (initWindow() == -1)
        return -1;

    unsigned int snowVBO, snowVAO;
    glGenVertexArrays(1, &snowVAO);
    glGenBuffers(1, &snowVBO);
    glBindVertexArray(snowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, snowVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(snowVertices), snowVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  //定义顶点数据解析方式，包括格式和位置
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    Shader snowShader("shader/mvp_shader.vs", "color_shader.fs");
    snowShader.setVec3("color", glm::vec3(0.0f));
    ParticleManager snowParticleManager(snowShader, snowVAO, 50);
    snowParticleManager.init();

    while (!glfwWindowShouldClose(window)) {
        //process input
        GLfloat currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        //render command
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // state set function
        glClear(GL_COLOR_BUFFER_BIT);  // state use function
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);  //设置透视矩阵
        snowShader.setMat4("view", view);
        snowShader.setMat4("projection", projection);
        snowParticleManager.update(deltaTime, currentFrame);
        snowParticleManager.draw();
        //double buffer
        glfwSwapBuffers(window);
        //event handle
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}



void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        surround = !surround;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}