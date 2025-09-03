#include "inputManager.hpp"

InputManager::InputManager(Camera* camera, GLFWwindow* window) : wireframe(false), rotate(true), mouseControl(false), m_camera(camera), m_window(window) {}
Camera* InputManager::getCamera() const { return m_camera; }

// process on key press
void InputManager::handleKey(int key, int action) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(m_window, true);
        if (key == GLFW_KEY_T) wireframe = !wireframe;
        if (key == GLFW_KEY_R) rotate = !rotate;
        if (key == GLFW_KEY_LEFT_CONTROL) {
            m_camera->firstMouse = true;
            mouseControl = !mouseControl;
            glfwSetInputMode(m_window, GLFW_CURSOR, mouseControl ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        }
    }
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
}

void InputManager::handleMouseButton(int button, int action) {
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            m_camera->firstMouse = true;
            mouseControl = !mouseControl;
            glfwSetInputMode(m_window, GLFW_CURSOR, mouseControl ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        }
    }
}

// process continuous input
void InputManager::processInput(float dt, glm::mat4& model) {

    // w,a,s,d for movement
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera->processKeyboard(FORWARD, dt);
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera->processKeyboard(BACKWARD, dt);
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera->processKeyboard(LEFT, dt);
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera->processKeyboard(RIGHT, dt);
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_camera->processKeyboard(DOWN, dt);
    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_camera->processKeyboard(UP, dt);

    // arrow keys for rolling and pitching
    if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
        m_camera->processKeyboard(ROLL_LEFT, dt);
    if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        m_camera->processKeyboard(ROLL_RIGHT, dt);
    if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
        m_camera->processKeyboard(PITCH_UP, dt);
    if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
        m_camera->processKeyboard(PITCH_DOWN, dt);

    // brackets for zooming (fov)
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
        m_camera->processScroll(-0.5f);
    if (glfwGetKey(m_window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
        m_camera->processScroll(0.5f);

    if (rotate)
        model = glm::rotate(model, dt * glm::radians(20.0f), glm::vec3(0,1,0));
}
