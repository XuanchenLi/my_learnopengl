#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Shader.h"
#include "../Camera.h"
#include <iostream>
#include <time.h>
#include <vector>


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
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool surround = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int initWindow() {
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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
}

float vertices[] = {
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
};

float axis[] = {
    0.0, 0.0, 0.0,
    5.0, 0.0, 0.0
};


int main() {
    if (initWindow() == -1)
        return -1;
    
    //雪花数据
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);  //生成顶点数组对象
    glGenBuffers(1, &VBO);  //生成顶点缓冲对象
    
    glBindVertexArray(VAO);  //绑定顶点数组对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  //绑定顶点缓冲对象
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //将数据传输至缓存

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  //定义顶点数据解析方式，包括格式和位置
    glEnableVertexAttribArray(0);


    Shader myShader("shader/lab1.vs", "shader/lab4.fs");
    time_t t;
    srand((unsigned) time(&t));
    std::vector<glm::vec3> poss;
    std::vector<glm::vec3> colors;
    for (int i = 0; i < 10; ++i) {
        poss.push_back(glm::vec3(rand() % 30 - 15, rand() % 30 - 15, rand() % 30 - 15));
        colors.push_back(glm::vec3(rand() % 1000 / (float)(1000.0), rand() % 1000 / (float)(1000.0), rand() % 1000 / (float)(1000.0)));
    }

    while (!glfwWindowShouldClose(window)) {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);  //处理用户输入
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 


        glm::mat4 view = glm::mat4(1.0);
        if (!surround) {
            view = camera.GetViewMatrix();
            
        }
        else 
            view = glm::lookAt(glm::vec3(8.0 * cos(currentFrame), 0.0, 8.0 * sin(currentFrame)), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
            
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);  //设置透视矩阵
        myShader.setMat4("view", view);
        myShader.setMat4("projection", projection);

        //绘制雪花
        myShader.use();
        glBindVertexArray(VAO);  //绑定顶点数组对象
        for (int j = 0; j < 10; ++j) {
            for (int i = 0; i < 6; ++i) {
            glm::mat4 model = glm::mat4(1.0);
            model = glm::translate(model, poss[j]);
            model = glm::rotate(model, currentFrame, glm::vec3(0.0f, 0.0f, 1.0f));  //随时间绕中心旋转
            model = glm::rotate(model, glm::radians(60.0f * i), glm::vec3(0.0, 0.0, 1.0f));
            myShader.setVec3("color", colors[j]);
            myShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 11);
            model = glm::scale(model, glm::vec3(1.0f, -1.0f, 1.0f));
            myShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 11);
        }
        }
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