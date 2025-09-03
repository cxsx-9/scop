#include "camera.hpp"

Camera::Camera() : Position(0,0,5), WorldUp(0,1,0), Yaw(-90), Pitch(0), Fov(45), MovementSpeed(2.5f) {}

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix() const {
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

glm::mat4 Camera::getProjection(float aspect, float nearP, float farP) const {
    return glm::perspective(glm::radians(Fov), aspect, nearP, farP);
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

    std::cout << "Yaw: " << Yaw << " Pitch: " << Pitch << std::endl;
}

void Camera::processScroll(float yoffset) {
    Fov -= yoffset;
    if(Fov < 1.0f) Fov = 1.0f;
    if(Fov > 90.0f) Fov = 90.0f;

    std::cout << "Fov: " << Fov << std::endl;
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = 2.5f * deltaTime;
    glm::vec3 front {
        cos(glm::radians(Yaw))*cos(glm::radians(Pitch)),
        sin(glm::radians(Pitch)),
        sin(glm::radians(Yaw))*cos(glm::radians(Pitch))
    };
    glm::vec3 Front = glm::normalize(front);
    glm::vec3 flatFront = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
    glm::vec3 Right = glm::normalize(glm::cross(flatFront, WorldUp));


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