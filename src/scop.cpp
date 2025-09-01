#include "scop.hpp"
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
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // set input callback for mouse and keyboard
    
    

    // set input callback for mouse and keyboard

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        exit(1);
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

void display(GLFWwindow* window, Shader shader, Model model, glm::mat4& modelMatrix, Renderer renderer, Camera camera) {
    float last = (float)glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float now = (float)glfwGetTime();
        float dt = now - last; last = now;
        
        glfwPollEvents();
        processInput(window, dt, modelMatrix);
        
        renderer.clear();

        glm::mat4 view = camera.getViewMatrix();
        float aspect = (gH==0) ? 1.0f : (float)gW/(float)gH;
        glm::mat4 proj = camera.getProjection(aspect);

        renderer.draw(model, shader, modelMatrix, view, proj);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}

void inputValidator(int ac, char **av){
    (void) av;
    if (ac != 2) {
        std::cerr << "Usage : ./scop file.obj" << std::endl;
        exit(1);
    }
    
    std::string path = av[1];
    if (path.length() >= 4 && path.substr(path.length() - 4) != ".obj")
    {
        std::cerr << "Invalid object file" << std::endl;
        exit(1);
    }

    std::ifstream file(path, std::ios::in);
    if(file.is_open()) {
        if (file.peek() == std::ifstream::traits_type::eof()) {
			std::cerr << "The object file is empty" << std::endl;
			file.close();
			exit(1);
		}
		file.close();
    } else {
        std::cerr << "Cannot open object file" << std::endl;
        file.close();
        exit(1);
    }
}

int main(int ac, char **av) {
    inputValidator(ac, av);
    initGLFW();

    Model model;
    model.loadObject(av[1]);

    GLFWwindow* window = createWindow(model.name);
    
    model.setup();
    Shader shader("src/shaders/vertexShader.glsl", "src/shaders/fragmentShader.glsl");

    Camera camera;
    Renderer renderer;
    glm::mat4 modelMatrix(1.0f);

    display(window, shader, model, modelMatrix, renderer, camera);
    
    return 0;
}
