#include "model.hpp"

Model::Model() {};

Model::~Model() {
    if (EBO) glDeleteBuffers(1,&EBO);
    if (VBO) glDeleteBuffers(1,&VBO);
    if (VAO) glDeleteVertexArrays(1,&VAO);
};

void Model::setup() {
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal));

    // texCoord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoord));
    glBindVertexArray(0);
};

void Model::bind() const { 
    glBindVertexArray(VAO);
}

void Model::loadObject(const std::string &path) {
     std::ifstream file(path, std::ios::in);
    if(path.empty() || !file.is_open()) {
        std::cerr << "[Error] Cannot open object file from : '" << path << "'" << std::endl;
        mockup(*this);
        return;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> normals;
    std::vector<FaceIndex> faces;

    std::string line;
    while (std::getline(file, line)) {
        std::string prefix;
        std::istringstream stream(line);
        stream >> prefix;

        if (line.empty() || prefix == "#") continue;

        if (prefix == "o") {
            stream >> this->name;
        } else if (prefix == "v") {
            glm::vec3 pos;
            stream >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        } else if (prefix == "vt") {
            glm::vec2 uv;
            stream >> uv.x >> uv.y;
            texcoords.push_back(uv);
        }
        else if (prefix == "vn") {
            glm::vec3 norm;
            stream >> norm.x >> norm.y >> norm.z;
            normals.push_back(norm);
        } else if (prefix == "f") {
            std::string v1,v2,v3;
            stream >> v1 >> v2 >> v3;
            std::array<std::string,3> face = {v1,v2,v3};

            for (auto &fv : face) {
                unsigned int vi=0, ti=0, ni=0;
                sscanf(fv.c_str(), "%d/%d/%d", &vi, &ti, &ni);

                Vertex vertex;
                vertex.position = positions[vi-1];
                vertex.texCoord = (ti>0 ? texcoords[ti-1] : glm::vec2(0.0f));
                vertex.normal   = (ni>0 ? normals[ni-1]   : glm::vec3(0.0f,0.0f,1.0f));

                this->vertices.push_back(vertex);
                this->indices.push_back((unsigned int)this->indices.size());
            }
        }
    }
    this->hasUV = !texcoords.empty();
    this->hasNormals = !normals.empty();
}

std::vector<Vertex> vts = {
    // +Z (front)
    {{-0.5f,-0.5f, 0.5f}, {0,0,1}, {0,0}},
    {{ 0.5f,-0.5f, 0.5f}, {0,0,1}, {0,0}},
    {{ 0.5f, 0.5f, 0.5f}, {0,0,1}, {0,0}},
    {{-0.5f, 0.5f, 0.5f}, {0,0,1}, {0,0}},

    // -Z (back)
    {{-0.5f,-0.5f,-0.5f}, {0,0,-1}, {0,0}},
    {{ 0.5f,-0.5f,-0.5f}, {0,0,-1}, {0,0}},
    {{ 0.5f, 0.5f,-0.5f}, {0,0,-1}, {0,0}},
    {{-0.5f, 0.5f,-0.5f}, {0,0,-1}, {0,0}},

    // +X (right)
    {{ 0.5f,-0.5f, 0.5f}, {1,0,0}, {0,0}},
    {{ 0.5f,-0.5f,-0.5f}, {1,0,0}, {0,0}},
    {{ 0.5f, 0.5f,-0.5f}, {1,0,0}, {0,0}},
    {{ 0.5f, 0.5f, 0.5f}, {1,0,0}, {0,0}},

    // -X (left)
    {{-0.5f,-0.5f,-0.5f}, {-1,0,0}, {0,0}},
    {{-0.5f,-0.5f, 0.5f}, {-1,0,0}, {0,0}},
    {{-0.5f, 0.5f, 0.5f}, {-1,0,0}, {0,0}},
    {{-0.5f, 0.5f,-0.5f}, {-1,0,0}, {0,0}},

    // +Y (top)
    {{-0.5f, 0.5f, 0.5f}, {0,1,0}, {0,0}},
    {{ 0.5f, 0.5f, 0.5f}, {0,1,0}, {0,0}},
    {{ 0.5f, 0.5f,-0.5f}, {0,1,0}, {0,0}},
    {{-0.5f, 0.5f,-0.5f}, {0,1,0}, {0,0}},

    // -Y (bottom)
    {{-0.5f,-0.5f,-0.5f}, {0,-1,0}, {0,0}},
    {{ 0.5f,-0.5f,-0.5f}, {0,-1,0}, {0,0}},
    {{ 0.5f,-0.5f, 0.5f}, {0,-1,0}, {0,0}},
    {{-0.5f,-0.5f, 0.5f}, {0,-1,0}, {0,0}},
};

std::vector<unsigned int> idx = {
    0,1,2,      2,3,0,      // +Z
    4,5,6,      6,7,4,      // -Z
    8,9,10,     10,11,8,    // +X
    12,13,14,   14,15,12,   // -X
    16,17,18,   18,19,16,   // +Y
    20,21,22,   22,23,20    // -Y
};

void mockup(Model &model) {
    model.name = "Opengl";
    model.vertices = vts;
    model.indices = idx;
}