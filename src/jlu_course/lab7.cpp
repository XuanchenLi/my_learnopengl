#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Shader.h"
#include "../Camera.h"
#include <iostream>
#include <time.h>
#include <vector>
#include <map>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



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
unsigned int loadTexture(char const *path);
unsigned int loadCubemap(std::vector<std::string>);

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//雪花顶点数据
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
    0.0, 0.0, 0.0,
};
//地板顶点数据
float mFloor[] = {
    //--位置坐标-----------纹理坐标---
    0.5f,  0.0f, 0.5f,  1.0f, 1.0f,   // 右上
    0.5f, 0.0f, -0.5f,  1.0f, 0.0f,   // 右下
    -0.5f, 0.0f, -0.5f,  0.0f, 0.0f,   // 左下
    -0.5f,  0.0f, 0.5f,  0.0f, 1.0f    // 左上
};
//天空盒顶点数据
float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
    };
//地板顶点索引
unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

float transparentBlock[] = {
        // 顶点坐标         //翻转纹理y坐标
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
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
    //地板数据
    unsigned int floorVBO, floorVAO;
    glGenVertexArrays(1, &floorVAO);  
    glGenBuffers(1, &floorVBO);  
    
    glBindVertexArray(floorVAO);  
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(mFloor), mFloor, GL_STATIC_DRAW);  
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);  
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));  
    glEnableVertexAttribArray(1);

    //地板 EBO/IBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //天空盒数据
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //透明物数据
    unsigned int transparentVAO, transparentVBO;
    glGenVertexArrays(1, &transparentVAO);
    glGenBuffers(1, &transparentVBO);
    glBindVertexArray(transparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentBlock), transparentBlock, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    //导入纹理
    unsigned int floorTex = loadTexture("../Texture/wall.jpg");
    unsigned int grassTex = loadTexture("../Texture/grass.png");

    std::vector<std::string> faces
    {
    "../Texture/skybox/right.jpg",
    "../Texture/skybox/left.jpg",
    "../Texture/skybox/top.jpg",
    "../Texture/skybox/bottom.jpg",
    "../Texture/skybox/front.jpg",
    "../Texture/skybox/back.jpg"
    };
    unsigned int skyboxTexture = loadCubemap(faces);
    


    Shader myShader("shader/lab1.vs", "shader/lab4.fs");
    Shader texShader("shader/lab5.vs", "shader/lab5.fs");
    Shader skyboxShader("shader/skybox.vs", "shader/skybox.fs");
    time_t t;
    srand((unsigned) time(&t));
    std::vector<glm::vec3> transparentPos;
    std::vector<glm::vec3> poss;
    std::vector<glm::vec3> colors;
    for (int i = 0; i < 10; ++i) {
        poss.push_back(glm::vec3(rand() % 30 - 15, rand() % 30 - 15, rand() % 30 - 15));
        transparentPos.push_back(glm::vec3(rand() % 20 - 10, rand() % 20 - 10, rand() % 20 - 10));
        colors.push_back(glm::vec3(rand() % 1000 / (float)(1000.0), rand() % 1000 / (float)(1000.0), rand() % 1000 / (float)(1000.0)));
        colors.push_back(glm::vec3(rand() % 1000 / (float)(1000.0), rand() % 1000 / (float)(1000.0), rand() % 1000 / (float)(1000.0)));
    }
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    std::map<float, glm::vec3> sortTrans;
    while (!glfwWindowShouldClose(window)) {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);  //处理用户输入
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        glDepthFunc(GL_LESS);
        myShader.use();
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
        glBindVertexArray(VAO);  //绑定顶点数组对象
        for (int j = 0; j < 10; ++j) {
            for (int i = 0; i < 6; ++i) {
            glm::mat4 model = glm::mat4(1.0);
            model = glm::translate(model, poss[j]);
            model = glm::rotate(model, currentFrame, glm::vec3(0.0f, 0.0f, 1.0f));  //随时间绕中心旋转
            model = glm::rotate(model, glm::radians(60.0f * i), glm::vec3(0.0, 0.0, 1.0f));
            myShader.setVec3("color", colors[2*j]);
            myShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 12);
            model = glm::scale(model, glm::vec3(1.0f, -1.0f, 1.0f));
            myShader.setVec3("color", colors[2*j + 1]);
            myShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 12);
        }
        }

        //绘制地板
        
        texShader.use();
        glBindVertexArray(floorVAO);
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, floorTex);
        texShader.setMat4("view", view);
        texShader.setMat4("projection", projection);
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                texShader.setMat4("model", glm::translate(glm::mat4(1.0), glm::vec3(i - 5, 0, j - 5)));
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  
            }        
        }
        glBindVertexArray(0);

        //绘制天空盒
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        glBindVertexArray(skyboxVAO);
        glm::mat4 staticView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.setMat4("view", staticView);
        skyboxShader.setMat4("projection", projection);
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        skyboxShader.setInt("skybox", 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //绘制透明物
        glDepthFunc(GL_LESS);
        texShader.use();
        sortTrans.clear();
        texShader.setMat4("view", view);
        texShader.setMat4("projection", projection);
        for (auto i : transparentPos) {
            float dis = glm::length(camera.Position - i);
            sortTrans[dis] = i;
        }
        glActiveTexture(0);
        glBindVertexArray(transparentVAO);
        glBindTexture(GL_TEXTURE_2D, grassTex);
        for (auto it = sortTrans.rbegin(); it != sortTrans.rend(); ++it) {
            texShader.setMat4("model", glm::scale(glm::translate(glm::mat4(1.0), (*it).second), glm::vec3(5)));
            glDrawArrays(GL_TRIANGLES, 0, 6);
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


unsigned int loadTexture(char const *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); ++i) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }else {
            std::cout << "Texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textureID;
}