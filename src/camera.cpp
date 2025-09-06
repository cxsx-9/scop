#include "camera.hpp"

Camera::Camera() : Position(0,0,5), WorldUp(0,1,0), Yaw(-90), Pitch(0), Fov(45), MovementSpeed(2.5f) {}

Camera::~Camera() {}

mymath::mat4 Camera::getViewMatrix() const {
    mymath::vec3 front{
        static_cast<float>(cos(mymath::radians(Yaw))*cos(mymath::radians(Pitch))),
        static_cast<float>(sin(mymath::radians(Pitch))),
        static_cast<float>(sin(mymath::radians(Yaw))*cos(mymath::radians(Pitch)))
    };
    mymath::vec3 Front = mymath::normalize(front);
    mymath::vec3 Right = mymath::normalize(mymath::cross(Front, WorldUp));
    mymath::vec3 Up    = mymath::normalize(mymath::cross(Right, Front));
    return mymath::lookAt(Position, Position + Front, Up);
}

mymath::mat4 Camera::getProjection(float aspect, float nearP, float farP) const {
    return mymath::perspective(mymath::radians(Fov), aspect, nearP, farP);
}

void Camera::processMouse(float xpos, float ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = (xpos - lastX) * sensitivity;
    float yoffset = (lastY - ypos) * sensitivity;
    lastX = xpos;
    lastY = ypos;

    Yaw += xoffset;
    Pitch += yoffset;
    if(Pitch > 89.0f) Pitch = 89.0f;
    if(Pitch < -89.0f) Pitch = -89.0f;
}

void Camera::processScroll(float yoffset) {
    Fov -= yoffset;
    if(Fov < 1.0f) Fov = 1.0f;
    if(Fov > 90.0f) Fov = 90.0f;
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = 2.5f * deltaTime;
    mymath::vec3 front {
        static_cast<float>(cos(mymath::radians(Yaw))*cos(mymath::radians(Pitch))),
        static_cast<float>(sin(mymath::radians(Pitch))),
        static_cast<float>(sin(mymath::radians(Yaw))*cos(mymath::radians(Pitch)))
    };
    mymath::vec3 Front = mymath::normalize(front);
    mymath::vec3 flatFront = mymath::normalize(mymath::vec3(Front.x, 0.0f, Front.z));
    mymath::vec3 Right = mymath::normalize(mymath::cross(flatFront, WorldUp));


    if (direction == FORWARD)
        Position += flatFront * velocity;
    if (direction == BACKWARD)
        Position -= flatFront * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;

    if (direction == UP)
        Position += WorldUp * velocity;
    if (direction == DOWN)
        Position -= WorldUp * velocity;

    float sensitivity = 20.0f;
    if (direction == PITCH_UP)
        Pitch += velocity * sensitivity;
    if (direction == PITCH_DOWN)
        Pitch -= velocity * sensitivity;
    if (direction == ROLL_LEFT)
        Yaw -= velocity * sensitivity;
    if (direction == ROLL_RIGHT)
        Yaw += velocity * sensitivity;
}