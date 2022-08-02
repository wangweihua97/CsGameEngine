#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "../Window.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    glm::mat4 Projection;
    glm::mat4 View;

    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);

    float FOV;
    float Aspect;
    float Near;
    float Far;
    Camera()
    {

    }
    Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up) : Position(position), Forward(forward), Up(up)
    {
        UpdateView();
    }


    void SetPerspective(float fov, float aspect, float nearP, float farP)
    {
        Projection = glm::perspective(fov, aspect, nearP, farP);
        FOV = fov;
        Aspect = aspect;
        Near = nearP;
        Far = farP;
    }

    void UpdateView()
    {
        View = glm::lookAt(Position, Position + Forward, Up);
    }
    

private:
    //void updateCameraVectors()
    //{
    //    // calculate the new Front vector
    //    glm::vec3 front;
    //    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    //    front.y = sin(glm::radians(Pitch));
    //    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    //    Front = glm::normalize(front);
    //    // also re-calculate the Right and Up vector
    //    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    //    Up = glm::normalize(glm::cross(Right, Front));
    //}
};
#endif
