#include "inputManager.hpp"

InputManager::InputManager(Camera* camera, GLFWwindow* window) {
    wireframe = false;
    rotate = true;
    mouseControl = false;
    moveMode = OBJECT;
    renderMode = MODE_GRAY;
    position = mymath::vec3(0.0f);
    rotation = mymath::mat4(1.0f);
    m_camera = camera;
    m_window = window;
    transitioning = false;
    prevMode = renderMode;

    showHelp();
}

Camera* InputManager::getCamera() const { return m_camera; }

void InputManager::showHelp() const {
    std::cout << "Controls:\n"
    << "  [WASD] Move " << (moveMode == CAMERA ? "Camera" : "Object") << "\n"
    << "  [Q/E]   Move " << (moveMode == CAMERA ? "Camera" : "Object") << " Up/Down\n"
    << "  [Arrow] Rotate Camera\n"
    << "  [Space/Shift] Move Camera Up/Down\n"
    << "  [[/]]   Zoom In/Out Camera\n"
    << "  [Left Click] Toggle Mouse Control\n"
    << "  [1]     Shading Mode: Gray\n"
    << "  [2]     Shading Mode: Colorful\n"
    << "  [3]     Shading Mode: Texture\n"
    << "  [0]     Switch Shading Mode\n"
    << "  [R]     Toggle Auto Rotation\n"
    << "  [T]     Toggle Wireframe\n"
    << "  [M]     Switch Mode (Camera/Object)\n"
    << "  [H]     Help\n"
    << "  [I]     Status\n"
    << "  [ESC]   Exit\n"
    << std::endl;
}

void InputManager::showStatus() const {
    std::cout << "Status: " << std::endl;
    std::cout << "  Move: " << (moveMode == CAMERA ? "Camera" : "Object") << std::endl;
    std::cout << "  Wireframe: " << (wireframe ? "ON" : "OFF") << std::endl;
    std::cout << "  Shading Mode: " << (renderMode == MODE_GRAY ? "Gray" : (renderMode == MODE_COLORFUL ? "Colorful" : "Texture")) << std::endl;
    std::cout << "  Auto Rotation: " << (rotate ? "ON" : "OFF") << std::endl;
    std::cout << "  Mouse Control: " << (mouseControl ? "ON" : "OFF") << std::endl;
    std::cout << std::endl;
}

// process on key press
void InputManager::handleKey(int key, int action) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(m_window, true);
        if (key == GLFW_KEY_H) showHelp();
        if (key == GLFW_KEY_I) showStatus();
        if (key == GLFW_KEY_T) {
            wireframe = !wireframe;
            std::cout << "Wireframe: " << (wireframe ? "ON" : "OFF") << std::endl;
        }
        if (key == GLFW_KEY_R) {
            rotate = !rotate;
            std::cout << "Auto Rotation: " << (rotate ? "ON" : "OFF") << std::endl;
        }
        if (key == GLFW_KEY_M) {
            moveMode = (moveMode == CAMERA) ? OBJECT : CAMERA;
            std::cout << "Move: " << (moveMode == CAMERA ? "Camera" : "Object") << std::endl;
        }
        if (key == GLFW_KEY_1) {
            if (renderMode != MODE_GRAY) {
                prevMode = renderMode;
                renderMode = MODE_GRAY;
                transitioning = true;
                mixFactor = 0.0f;
            }
            std::cout << "Shading Mode: Gray" << std::endl;
        }
        if (key == GLFW_KEY_2) {
            if (renderMode != MODE_COLORFUL) {
                prevMode = renderMode;
                renderMode = MODE_COLORFUL;
                transitioning = true;
                mixFactor = 0.0f;
            }
            std::cout << "Shading Mode: Colorful" << std::endl;
        }
        if (key == GLFW_KEY_3) {
            if (renderMode != MODE_TEXTURE) {
                prevMode = renderMode;
                renderMode = MODE_TEXTURE;
                transitioning = true;
                mixFactor = 0.0f;
            }
            std::cout << "Shading Mode: Texture" << std::endl;
        }
        if (key == GLFW_KEY_0) {
            prevMode = renderMode;
            renderMode = (renderMode == MODE_COLORFUL) ? MODE_TEXTURE : MODE_COLORFUL;
            transitioning = true;
            mixFactor = 0.0f;
            std::cout << "Shading Mode: " << (renderMode == MODE_COLORFUL ? "Colorful" : "Texture") << std::endl;
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
void InputManager::processInput(float dt, mymath::mat4& model) {

    // Object movement
    if (moveMode == OBJECT) {
        // WASDQE
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
            position += mymath::vec3(0, 0, -1) * dt;
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
            position += mymath::vec3(0, 0, 1) * dt;
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
            position += mymath::vec3(-1, 0, 0) * dt;
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
            position += mymath::vec3(1, 0, 0) * dt;
        if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
            position += mymath::vec3(0, 1, 0) * dt;
        if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
            position += mymath::vec3(0, -1, 0) * dt;
    }

    // Camera movement
    if (moveMode == CAMERA) {
        // WASDQE Shift Space
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
            m_camera->processKeyboard(FORWARD, dt);
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
            m_camera->processKeyboard(BACKWARD, dt);
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
            m_camera->processKeyboard(LEFT, dt);
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
            m_camera->processKeyboard(RIGHT, dt);
        if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
            m_camera->processKeyboard(UP, dt);
        if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            m_camera->processKeyboard(DOWN, dt);
        if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
            m_camera->processKeyboard(UP, dt);
        if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
            m_camera->processKeyboard(DOWN, dt);

        // arrow keys
        if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
            m_camera->processKeyboard(ROLL_LEFT, dt);
        if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            m_camera->processKeyboard(ROLL_RIGHT, dt);
        if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
            m_camera->processKeyboard(PITCH_UP, dt);
        if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
            m_camera->processKeyboard(PITCH_DOWN, dt);

        // fov adjust with '[' and ']'
        if (glfwGetKey(m_window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
            m_camera->processScroll(-0.5f);
        if (glfwGetKey(m_window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
            m_camera->processScroll(0.5f);
    }

    // auto rotation
    if (rotate)
        rotation = mymath::rotate(rotation, dt * mymath::radians(20.0f), mymath::vec3(0, 1, 0));
    
    model = mymath::mat4(1.0f);
    model = mymath::translate(model, position);
    model *= rotation;

    if (transitioning) {
        float speed = 150.0f * dt;
        mixFactor += dt * speed;
        if (mixFactor >= 1.0f) {
            mixFactor = 1.0f;
            transitioning = false;
        }
    }
}
