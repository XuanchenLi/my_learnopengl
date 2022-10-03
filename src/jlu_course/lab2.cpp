#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Shader.h"
#include "../Camera.h"
#include <iostream>


// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
GLFWwindow* window;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool viewFromX = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);

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

    //坐标轴数据
    unsigned int axisVBO, axisVAO;
    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);
    
    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axis), axis, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    Shader myShader("shader/lab1.vs", "shader/lab1.fs");
    Shader axisShader("shader/lab1.vs", "shader/axisShader.fs");
    
    while (!glfwWindowShouldClose(window)) {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        //render command
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        glm::mat4 view = glm::mat4(1.0);
        if (viewFromX) {
            view = glm::lookAt(glm::vec3(8.0, 0.0, 0.0), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
        }
        else 
            view = glm::lookAt(glm::vec3(8.0 * cos(currentFrame), 0.0, 8.0 * sin(currentFrame)), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
            //view = glm::lookAt(glm::vec3(0.0, 0.0, 8.0), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
        //glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);  //设置透视矩阵
        myShader.setMat4("view", view);
        myShader.setMat4("projection", projection);

        
        //绘制坐标
        axisShader.use();
        glBindVertexArray(axisVAO);
        axisShader.setMat4("view", view);
        axisShader.setMat4("projection", projection);
        // x轴
        glm::mat4 axisModel = glm::mat4(1.0f);
        axisShader.setMat4("model", axisModel);
        axisShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
        glDrawArrays(GL_LINE_STRIP, 0, 2);
        // y轴
        axisModel = glm::rotate(axisModel, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        axisShader.setMat4("model", axisModel);
        axisShader.setVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
        glDrawArrays(GL_LINE_STRIP, 0, 2);
        // z轴
        axisModel = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        axisShader.setMat4("model", axisModel);
        axisShader.setVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));
        glDrawArrays(GL_LINE_STRIP, 0, 2);


        //绘制雪花
        myShader.use();
        glBindVertexArray(VAO);  //绑定顶点数组对象
        for (int i = 0; i < 6; ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, currentFrame, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(60.0f * i), glm::vec3(0.0, 0.0, 1.0f));
            myShader.setMat4("model", model);
            glDrawArrays(GL_LINE_STRIP, 0, 11);
            model = glm::scale(model, glm::vec3(1.0f, -1.0f, 1.0f));
            myShader.setMat4("model", model);
            glDrawArrays(GL_LINE_STRIP, 0, 11);
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
        viewFromX = !viewFromX;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
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