#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>


enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW         = 90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.08f;
const float ZOOM        =  45.0f;



class Camera
{
public:

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;


    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, 1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        Up = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        Up = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, WorldUp);
    }

 
    void ProcessKeyboard(Camera_Movement direction, float deltaTime, GLboolean FPSmodel = false)
    {
        float velocity = MovementSpeed * deltaTime;
        if (!FPSmodel) {
            if (direction == FORWARD)
                Position += Front * velocity;
            if (direction == BACKWARD)
                Position -= Front * velocity;
            if (direction == LEFT)
                Position -= Right * velocity;
            if (direction == RIGHT)
                Position += Right * velocity;
        }else {
            glm::vec3 goFront = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
            glm::vec3 goRight = glm::normalize(glm::cross(goFront, WorldUp));
            if (direction == FORWARD)
                Position += goFront * velocity;
            if (direction == BACKWARD)
                Position -= goFront * velocity;
            if (direction == LEFT)
                Position -= goRight * velocity;
            if (direction == RIGHT)
                Position += goRight * velocity;
        }
        
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;
        //std::cout<<xoffset<<" "<<yoffset<<std::endl;

        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }


        updateCameraVectors();
    }

  
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f; 
    }

    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        
        Right = glm::normalize(glm::cross(Front, WorldUp)); 
        Up    = glm::normalize(glm::cross(Right, Front));
    }

    void updateCameraVectors2()
    {

        // Calculate the new Front vector
        glm::mat4 rotateMat = glm::mat4(1.0f);
        rotateMat = glm::rotate(rotateMat, glm::radians(-Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateMat = glm::rotate(rotateMat, glm::radians(Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::vec4 front = rotateMat * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
        Front = glm::normalize(glm::vec3(front.x, front.y, front.z));
     
        Right = glm::normalize(glm::cross(Front, WorldUp)); 
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif