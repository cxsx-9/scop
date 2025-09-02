#pragma once

#include "scop.hpp"
// #include "math.hpp"

class Camera {
public:
    glm::vec3 Position{0.0f, 0.0f, 5.0f};
    glm::vec3 WorldUp{0.0f, 1.0f, 0.0f};
    float Yaw{-90.0f};
    float Pitch{0.0f};
    float Fov{45.0f};

    glm::mat4 getViewMatrix() const {
        glm::vec3 front{
            cos(glm::radians(Yaw))*cos(glm::radians(Pitch)),
            sin(glm::radians(Pitch)),
            sin(glm::radians(Yaw))*cos(glm::radians(Pitch))
        };
        glm::vec3 Front = glm::normalize(front);
        glm::vec3 Right = glm::normalize(glm::cross(Front, WorldUp));
        glm::vec3 Up    = glm::normalize(glm::cross(Right, Front));
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 getProjection(float aspect, float nearP=0.1f, float farP=100.0f) const {
        return glm::perspective(glm::radians(Fov), aspect, nearP, farP);
    }
};
