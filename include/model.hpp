#pragma once

#include "scop.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;

    bool operator==(const Vertex &other) const {
    return position == other.position &&
            normal   == other.normal &&
            texCoord == other.texCoord;
    }
};

struct VertexHasher {
    size_t operator()(const Vertex& v) const {
        return std::hash<int>{}(int(v.position.x*1000)) ^
               std::hash<int>{}(int(v.position.y*1000)) ^
               std::hash<int>{}(int(v.position.z*1000));
    }
};

struct VertexEqual {
    bool operator()(const Vertex& a, const Vertex& b) const {
        auto equalFloat = [](float x, float y) {
            return std::abs(x - y) < 0.0001f;
        };

        return
            equalFloat(a.position.x, b.position.x) &&
            equalFloat(a.position.y, b.position.y) &&
            equalFloat(a.position.z, b.position.z) &&

            equalFloat(a.normal.x, b.normal.x) &&
            equalFloat(a.normal.y, b.normal.y) &&
            equalFloat(a.normal.z, b.normal.z) &&

            equalFloat(a.texCoord.x, b.texCoord.x) &&
            equalFloat(a.texCoord.y, b.texCoord.y);
    }
};

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
    int v, t, n;
};

class Model{
    public:
        std::string name;
        GLuint VAO=0, VBO=0, EBO=0;
        std::vector<Vertex>         vertices;
        std::vector<unsigned int>   indices;
        bool hasUV      = false;

        Material material;

        Model();
        ~Model();

        void setup();
        void bind() const;
        void loadObject(const std::string &path);
        void centerAndNormalize(float targetExtent, float margin);
        FaceIndex parseFaceToken(const std::string &token);
        void computeNormals();
        void deduplicateVertices();
};

void mockup(Model &model);