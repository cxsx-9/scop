#pragma once

#include "scop.hpp"
// #include "math.hpp"

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    PITCH_UP,
    PITCH_DOWN,
    ROLL_LEFT,
    ROLL_RIGHT
};

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;
    float Fov;
    float MovementSpeed;

    bool firstMouse = true;
    float lastX = 400, lastY = 300;
    float sensitivity = 0.1f;

    Camera();
    ~Camera();

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjection(float aspect, float nearP=0.1f, float farP=100.0f) const;

    void processMouse(float xpos, float ypos);
    void processScroll(float yoffset);
    void processKeyboard(CameraMovement direction, float deltaTime);
};
