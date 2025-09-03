#pragma once

#include "scop.hpp"
#include "camera.hpp"

class Camera;

class InputManager {
    public:
        bool wireframe;
        bool rotate;
        bool mouseControl;
        
        InputManager(Camera* camera, GLFWwindow* window);
        
        Camera* getCamera() const;
        void handleKey(int key, int action);
        void handleMouseButton(int button, int action);
        void processInput(float dt, glm::mat4& model);

    private:
        Camera* m_camera;
        GLFWwindow* m_window;
};
