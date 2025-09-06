#pragma once

#include "scop.hpp"
#include "camera.hpp"

class Camera;

enum MovementMode {
    CAMERA,
    OBJECT
};

enum RenderMode {
    MODE_GRAY = 1,
    MODE_COLORFUL = 2,
    MODE_TEXTURE = 3
};

enum RotationInput {
    ROTATE_X_POS,
    ROTATE_X_NEG,
    ROTATE_Y_POS,
    ROTATE_Y_NEG,
    ROTATE_Z_POS,
    ROTATE_Z_NEG,
    ROTATE_ALL,
    NONE
};

class InputManager {
    public:
        InputManager(Camera* camera, GLFWwindow* window);

        bool wireframe;
        bool rotate;
        bool mouseControl;
        MovementMode moveMode;
        RenderMode renderMode;
        mymath::vec3 position;
        mymath::mat4 rotation;
        float mixFactor;
        bool transitioning;
        int prevMode;
        RotationInput rotationInput;

        Camera* getCamera() const;
        void handleKey(int key, int action);
        void handleMouseButton(int button, int action);
        void processInput(float dt, mymath::mat4& model);

        void showStatus() const;
        void showHelp() const;

    private:
        Camera* m_camera;
        GLFWwindow* m_window;
};
