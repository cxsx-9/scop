#include "scop.hpp"
#include "objectLoader.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "renderer.hpp"

static int gW=800, gH=600;
static bool gWire=false;

void initGLFW(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for macOS
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void) window;
    glViewport(0, 0, width, height);
}

GLFWwindow* createWindow(const std::string &name)
{
    GLFWwindow* window;
    window = glfwCreateWindow(gW, gH, name.c_str(), NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // set input callback for mouse and keyboard
    
    

    // set input callback for mouse and keyboard

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glEnable(GL_DEPTH_TEST);
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << "\n";

    return window;
}

static void processInput(GLFWwindow* win, float dt, glm::mat4& model) {
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(win, true);

    if (glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS) {
        if (!gWire) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); gWire = true; }
    }
    if (glfwGetKey(win, GLFW_KEY_T) == GLFW_PRESS) {
        if (gWire) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); gWire = false; }
    }

    model = glm::rotate(model, dt * glm::radians(20.0f), glm::vec3(0,1,0));
}

void display(GLFWwindow* window, Shader shader, Model meshModel, glm::mat4& model, Renderer renderer, Camera camera) {
    float last = (float)glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float now = (float)glfwGetTime();
        float dt = now - last; last = now;
        
        glfwPollEvents();
        processInput(window, dt, model);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        float aspect = (gH==0) ? 1.0f : (float)gW/(float)gH;
        glm::mat4 proj = camera.getProjection(aspect);

        renderer.draw(meshModel, shader, model, view, proj, camera.Position);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}



int main() {
    
    std::string path = ""; // mock

    initGLFW();

    // readfile for get name
    Model meshModel;
    objectLoader(path, meshModel);
    GLFWwindow* window = createWindow(meshModel.name);
    
    meshModel.setup();
    Shader shader("src/shaders/vertexShader.glsl", "src/shaders/fragmentShader.glsl");

    
    Camera camera;
    Renderer renderer;
    glm::mat4 model(1.0f);

    display(window, shader, meshModel, model, renderer, camera);
    
    return 0;
}
