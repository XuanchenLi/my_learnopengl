#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <map>
#include <glm/gtx/string_cast.hpp>
#include "ParticleManager.h"
#include "utils/Shader.h"
#include "utils/Camera.h"
#include "TextureLoader.h"
#include "GameObject.h"
#include "light/DirLight.h"
#include "light/PointLight.h"
#include "light/SpotLight.h"


// settings
unsigned int SCR_WIDTH = 1920;
unsigned int SCR_HEIGHT = 1080;
GLFWwindow* window;

// camera
Camera camera(glm::vec3(0.0f, 3.1f, 1.5f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat interval = 0.0f;

glm::vec3 playerPosition(0.0f);
//GLfloat playerYaw = 90.0f;
glm::vec3 playerFront(0.0f, 0.0f, 1.0f);
bool firstSwitch = false;
bool surround = true;

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
//地板顶点数据
float mFloor[] = {
    //--位置坐标-----------纹理坐标---
    0.5f,  0.0f, 0.5f,  1.0f, 1.0f,   // 右上
    0.5f, 0.0f, -0.5f,  1.0f, 0.0f,   // 右下
    -0.5f, 0.0f, -0.5f,  0.0f, 0.0f,   // 左下
    -0.5f,  0.0f, 0.5f,  0.0f, 1.0f    // 左上
};
//地板顶点索引
unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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

void setupLightData(Shader& ourShader, DirLight& dir, PointLight& point, SpotLight& spot) {
    ourShader.use();
    ourShader.setFloat("material.shininess", 32.0f);
    ourShader.setVec3("dirLight.direction", dir.direction);
    ourShader.setVec3("dirLight.ambient", dir.ambient);
    ourShader.setVec3("dirLight.diffuse", dir.diffuse);
    ourShader.setVec3("dirLight.specular", dir.specular);

    ourShader.setVec3("pointLights[0].position", point.position);
    ourShader.setVec3("pointLights[0].ambient", point.ambient);
    ourShader.setVec3("pointLights[0].diffuse", point.diffuse);
    ourShader.setVec3("pointLights[0].specular", point.specular);
    ourShader.setFloat("pointLights[0].constant", point.constant);
    ourShader.setFloat("pointLights[0].linear", point.linear);
    ourShader.setFloat("pointLights[0].quadratic", point.quadratic);
    
    // spotLight
    ourShader.setVec3("spotLight.position", spot.position);
    ourShader.setVec3("spotLight.direction", spot.direction);
    ourShader.setVec3("spotLight.ambient", spot.ambient);
    ourShader.setVec3("spotLight.diffuse", spot.diffuse);
    ourShader.setVec3("spotLight.specular", spot.specular);
    ourShader.setFloat("spotLight.constant", spot.constant);
    ourShader.setFloat("spotLight.linear", spot.linear);
    ourShader.setFloat("spotLight.quadratic", spot.quadratic);
    ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(spot.cutOff)));
    ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(spot.outerCutOff)));  
}

int main() {

    if (initWindow() == -1)
        return -1;
    //雪地VAO VBO
    unsigned int snowVBO, snowVAO;
    glGenVertexArrays(1, &snowVAO);
    glGenBuffers(1, &snowVBO);
    glBindVertexArray(snowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, snowVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(snowVertices), snowVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  //定义顶点数据解析方式，包括格式和位置
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
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
    unsigned int floorEBO;
    glGenBuffers(1, &floorEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
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

    //模型
    GameObject snowman(glm::vec3(0.0f), Model("model/snowman/snowman.obj"));
    GameObject tree(glm::vec3(3.0f, 0.0f, 3.0f), Model("model/tree/Tree.obj"));
    GameObject doll(glm::vec3(-3.0f, 0.0f, -3.0f), Model("model/TheDoll/doll.obj"));
    GameObject maria(glm::vec3(-3.0f, 0.0f, 3.0f), Model("model/LadyMaria/c4520 lady maria.obj"));
    doll.Scale = 2.0f;
    maria.Scale = 2.0f;

    //雪花粒子着色器
    Shader snowShader("shader/instanced_mvp_shader.vs", "shader/color_shader.fs");
    snowShader.use();
    snowShader.setVec3("color", glm::vec3(1.0f));
    //
    Shader floorShader("shader/texture_shader.vs", "shader/texture_shader.fs");
    Shader skyboxShader("shader/skybox.vs", "shader/skybox.fs");
    //模型着色器
    Shader modelShader("shader/model_loading.vs", "shader/model_loading.fs");
    DirLight dirLight(
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.3f),
        glm::vec3(0.4f),
        glm::vec3(0.5f)
    );
    PointLight pointLight(
        glm::vec3(0.0f, 5.0f, 0.0f),
        1.0f, 0.09f, 0.032f,
        glm::vec3(0.1f),
        glm::vec3(0.8),
        glm::vec3(1.0f)
    );
    SpotLight spotLight(
        glm::vec3(5.0f, 1.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        12.5f, 15.0f,
        1.0f, 0.09f, 0.032f,
        glm::vec3(0.1f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    setupLightData(modelShader, dirLight, pointLight, spotLight);

    //雪花粒子发生器
    ParticleManager snowParticleManager(snowShader, snowVAO, 4000);
    snowParticleManager.init();

    GLuint floorTex = TextureLoader::loadTexture("texture/snowplain.jpg");
    std::vector<std::string> skyFaces
    {
    "texture/skybox/right.png",
    "texture/skybox/left.png",
    "texture/skybox/top.png",
    "texture/skybox/bottom.png",
    "texture/skybox/front.png",
    "texture/skybox/back.png"
    };
    unsigned int skyboxTexture = TextureLoader::loadCubemap(skyFaces);
    
    while (!glfwWindowShouldClose(window)) {
        //process input
        GLfloat currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        //render command
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // state set function
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // state use function
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);  //设置透视矩阵
        //绘制雪花粒子
        snowShader.use();
        snowShader.setMat4("view", view);
        snowShader.setMat4("projection", projection);
        //snowShader.setVec3("color", glm::vec3(1.0f));
        snowParticleManager.update(deltaTime, currentFrame);
        snowParticleManager.draw();

        //绘制地板
        floorShader.use();
        glBindVertexArray(floorVAO);
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, floorTex);
        floorShader.setMat4("view", view);
        floorShader.setMat4("projection", projection);
        for (int i = 0; i < 40; ++i) {
            for (int j = 0; j < 40; ++j) {
                floorShader.setMat4("model", glm::translate(glm::mat4(1.0), glm::vec3(i - 20, 0, j - 20)));
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  
            }        
        }
        //绘制模型
        modelShader.use();
        modelShader.setMat4("view", view);
        modelShader.setMat4("projection", projection);
        modelShader.setVec3("viewPos", camera.Position);
        snowman.SetPosition(playerPosition);
        GLfloat roa = glm::atan(playerFront.x, playerFront.z);
        //std::cout<<glm::radians(glm::atan(1.0, 0.0))<<std::endl;
        //std:cout<<glm::radians(roa)<<std::endl;
        snowman.SetRotation(roa);
        snowman.Draw(modelShader);
        maria.Draw(modelShader);
        doll.Draw(modelShader);
        tree.Draw(modelShader);
        //绘制天空盒
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        glBindVertexArray(skyboxVAO);
        glm::mat4 staticView = glm::mat4(glm::mat3(camera.GetViewMatrix()));  //消除平移分量
        skyboxShader.setMat4("view", staticView);
        skyboxShader.setMat4("projection", projection);
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        skyboxShader.setInt("skybox", 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);

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
    //std::cout<<glm::to_string(playerPosition)<<std::endl;
    interval += deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (interval >= 0.5) {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != 0) {
        surround = !surround;
        if (surround == false) {
            //第三人称切换至第一人称
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            camera.Yaw = glm::atan(playerFront.z, playerFront.x) * 180.0f / 3.14159f;
            camera.Pitch = 0.0f;
            camera.updateCameraVectors();
        }
        else {
            //第一人称切换至第三人称
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        firstSwitch = false;
        interval = 0;
        }
    }

    if (surround == false) {
        camera.Position = glm::vec3(playerPosition.x, playerPosition.y + 3.1, playerPosition.z);
        camera.Position += glm::vec3(camera.Front.x * 1.5, 0.0f, camera.Front.z * 1.5);
        playerFront = glm::normalize(glm::vec3(camera.Front.x, 0.0f, camera.Front.z));
    }else {
        camera.Position = glm::vec3(playerPosition.x, playerPosition.y + 10, playerPosition.z);
        camera.Position -= glm::vec3(playerFront.x * 15, 0.0f, playerFront.z * 15);
        camera.Front = glm::normalize(playerPosition - camera.Position);
        camera.Yaw = glm::atan(camera.Front.z, camera.Front.x) * 180.0f / 3.14159f;
        camera.Pitch = glm::asin(camera.Front.z) * 180.0f / 3.14159f;;
        camera.Right = glm::normalize(glm::cross(camera.Front, glm::vec3(0.0f, 1.0f, 0.0f)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        camera.Up    = glm::normalize(glm::cross(camera.Right, camera.Front));
    }

    //glm::vec3 playerFront = glm::vec3(cos(glm::radians(dir)), 0.0f, sin(glm::radians(dir)));
    glm::vec3 playerRight = glm::cross(playerFront, glm::vec3(0.0f, 1.0f, 0.0f));


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime, true);
        playerPosition += playerFront * deltaTime * 2.5f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime, true);
        playerPosition -= playerFront * deltaTime * 2.5f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime, true);
        playerPosition -= playerRight * deltaTime * 2.5f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime, true);
        playerPosition += playerRight * deltaTime * 2.5f;
    }

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
    //std::cout<<xoffset<<std::endl;
    lastX = xpos;
    lastY = ypos;
    if (surround == false)
        camera.ProcessMouseMovement(xoffset, yoffset);
    else {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != 0) {
            glm::mat4 rotateMatrix = glm::mat4(1.0f);
            rotateMatrix = glm::rotate(rotateMatrix, glm::radians(-xoffset), glm::vec3(0.0f, 1.0f, 0.0f));
            playerFront = glm::mat3(rotateMatrix) * playerFront;
        }
    }
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}