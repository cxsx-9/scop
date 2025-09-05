#include "scop.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "renderer.hpp"
#include "inputManager.hpp"

static int gW=800, gH=600;
static InputManager *gInputManager;

void initGLFW(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for macOS
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    (void) window;
    gW = (width > 0) ? width : 1;
    gH = (height > 0) ? height : 1;
    glViewport(0, 0, gW, gH);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void) window; (void) scancode; (void) action; (void) mods;
    gInputManager->handleKey(key, action);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    (void) window;
    if (gInputManager->mouseControl)
        gInputManager->getCamera()->processMouse((float)xpos, (float)ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    (void) window; (void) xoffset; (void) yoffset;
    gInputManager->getCamera()->processScroll((float)yoffset);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    (void) window; (void) button; (void) action; (void) mods;
    gInputManager->handleMouseButton(button, action);
}

GLFWwindow* createWindow(const std::string &name)
{
    GLFWwindow* window;
    window = glfwCreateWindow(gW, gH, name.c_str(), NULL, NULL);
    if (!window)
    {
        std::cerr << "[Error] Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[Error] Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        exit(1);
    }
    glEnable(GL_DEPTH_TEST);
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << "\n";

    return window;
}

void display(GLFWwindow* window, Shader& shader, Model& model, mymath::mat4& modelMatrix, Renderer& renderer) {
    float last = (float)glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float now = (float)glfwGetTime();
        float dt = now - last; last = now;
        
        glfwPollEvents();
        gInputManager->processInput(dt, modelMatrix);

        renderer.clear();

        int fbW, fbH;
        glfwGetFramebufferSize(window, &fbW, &fbH);
        float aspect = (fbH == 0) ? 1.0f : (float)fbW / (float)fbH;
        mymath::mat4 view = gInputManager->getCamera()->getViewMatrix();
        mymath::mat4 proj = gInputManager->getCamera()->getProjection(aspect);

        renderer.draw(model, shader, modelMatrix, view, proj);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}

int main(int ac, char **av) {
    inputValidator(ac, av);
    initGLFW();

    Model model;
    model.loadObject(av[1]);
    if (ac == 3 && textureValidator(av[2]) == 0) model.loadTexture(av[2]);
    model.showInfo();

    GLFWwindow* window = createWindow(model.name);
    
    model.setup();
    Shader shader("src/shaders/vertexShader.glsl", "src/shaders/fragmentShader.glsl");

    Camera* camera = new Camera();
    InputManager inputManager(camera, window);
    gInputManager = &inputManager;

    Renderer renderer(inputManager);
    mymath::mat4 modelMatrix(1.0f);

    display(window, shader, model, modelMatrix, renderer);
    
    return 0;
}
