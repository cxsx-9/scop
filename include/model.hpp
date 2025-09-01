#pragma once

#include "scop.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

// struct Material {
//     std::string name;

//     // glm::vec3 ambientColor    = glm::vec3(0.0f); // Ka
//     glm::vec3 diffuseColor    = glm::vec3(0.64f); // Kd
//     // glm::vec3 specularColor   = glm::vec3(0.5f); // Ks
//     // float shininess           = 32.0f;           // Ns
//     // float opacity             = 1.0f;            // d
//     // int illumModel            = 2;               // illum
//     // float opticalDensity      = 1.0f;            // Ni (Index of Refraction)

//     GLuint diffuseMap  = 0;
//     // GLuint specularMap = 0;
//     // GLuint normalMap   = 0;
// };

struct Material {
    std::string name;
    glm::vec3 diffuseColor;
    GLuint diffuseMap;

    Material()
        : name(""),
          diffuseColor(1, 1, 1),
          diffuseMap(0) {}
};

struct FaceIndex {
    unsigned int v, t, n;
};

class Model{
    public:
        std::string name;
        GLuint VAO=0, VBO=0, EBO=0;
        std::vector<Vertex>         vertices;
        std::vector<unsigned int>   indices;
        bool hasUV      = false;
        bool hasNormals = false;

        Material material;


        void bind() const;
        void loadObject(const std::string &path);

        Model();
        ~Model();

        void setup();
};

void mockup(Model &model);