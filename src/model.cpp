#include "model.hpp"

static void showInfo(const Model &model);

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

void Model::centerAndNormalize(float targetExtent = 2.0f, float margin = 0.9f) {
    if (vertices.empty()) return;

    glm::vec3 minp(FLT_MAX);
    glm::vec3 maxp(-FLT_MAX);

    for (auto &v : vertices) {
        minp = glm::min(minp, v.position);
        maxp = glm::max(maxp, v.position);
    }

    glm::vec3 center = 0.5f * (minp + maxp);
    glm::vec3 size   = maxp - minp;
    float maxExtent  = std::max(size.x, std::max(size.y, size.z));

    float scale = (targetExtent / maxExtent) * margin;

    for (auto &v : vertices) {
        v.position = (v.position - center) * scale;
    }

    for (auto &v : vertices) {
        v.normal = glm::normalize(v.normal);
    }
}

FaceIndex Model::parseFaceToken(const std::string &token) {
    int v=0, t=-1, n=-1;
    size_t p1 = token.find('/');
    if (p1 == std::string::npos) {
        v = std::stoi(token);
    } else {
        size_t p2 = token.find('/', p1 + 1);
        v = std::stoi(token.substr(0, p1));
        if (p2 == std::string::npos) {
            if (p1 + 1 < token.size())
                t = std::stoi(token.substr(p1 + 1));
        } else {
            if (p2 != p1 + 1) {
                t = std::stoi(token.substr(p1 + 1, p2 - p1 - 1));
            }
            if (p2 + 1 < token.size()) {
                n = std::stoi(token.substr(p2 + 1));
            }
        }
    }
    FaceIndex out;
    out.v = (v > 0 ? v - 1 : -1);
    out.t = (t > 0 ? t - 1 : -1);
    out.n = (n > 0 ? n - 1 : -1);
    return out;
}

void Model::computeNormals() {
    for (size_t i = 0; i < this->indices.size(); i += 3) {
        Vertex &v0 = this->vertices[this->indices[i]];
        Vertex &v1 = this->vertices[this->indices[i + 1]];
        Vertex &v2 = this->vertices[this->indices[i + 2]];

        glm::vec3 edge1 = v1.position - v0.position;
        glm::vec3 edge2 = v2.position - v0.position;
        glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

        v0.normal += faceNormal;
        v1.normal += faceNormal;
        v2.normal += faceNormal;
    }
    for (auto &v : this->vertices) {
        v.normal = glm::normalize(v.normal);
    }
}

void Model::deduplicateVertices() {
    std::vector<Vertex> newVertices;
    std::vector<unsigned int> newIndices;

    std::unordered_map<Vertex, unsigned int, VertexHasher, VertexEqual> vertexMap;

    for (unsigned int idx : indices) {
        const Vertex& v = vertices[idx];

        auto it = vertexMap.find(v);
        if (it != vertexMap.end()) {
            newIndices.push_back(it->second);
        } else {
            unsigned int newIndex = static_cast<unsigned int>(newVertices.size());
            newVertices.push_back(v);
            newIndices.push_back(newIndex);
            vertexMap[v] = newIndex;
        }
    }

    vertices = std::move(newVertices);
    indices = std::move(newIndices);
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
        } else if (prefix == "vn") {
            glm::vec3 norm;
            stream >> norm.x >> norm.y >> norm.z;
            normals.push_back(norm);
        } else if (prefix == "f") {
            std::vector<unsigned int> faceIndices;
            std::string token;
            while (stream >> token) {
                try {
                    FaceIndex idx = parseFaceToken(token);
                    // std::cout << "Parsing face token: " << token << "-> " << idx.v << ", " << idx.t << ", " << idx.n << std::endl; // Debug
                    Vertex vertex;
                    vertex.position = positions[idx.v];
                    vertex.texCoord = (idx.t >= 0 && idx.t < (int)texcoords.size()) ? texcoords[idx.t] : glm::vec2(0.0f);
                    vertex.normal   = (idx.n >= 0 && idx.n < (int)normals.size()) ? normals[idx.n] : glm::vec3(0.0f);

                    this->vertices.push_back(vertex);
                    faceIndices.push_back(static_cast<unsigned int>(this->vertices.size() - 1));
                } catch (const std::exception &e) {
                    std::cerr << "[Warn] failed to parse face token '" << token << "': " << e.what() << std::endl;
                }
            }

            // triangulate (triangle fan)
            for (size_t i = 1; i + 1 < faceIndices.size(); ++i) {
                this->indices.push_back(faceIndices[0]);
                this->indices.push_back(faceIndices[i]);
                this->indices.push_back(faceIndices[i + 1]);
            }
        }
    }
    
    this->hasUV = !texcoords.empty();
    if (normals.empty()) {
        computeNormals();
    }
    deduplicateVertices();
    this->centerAndNormalize();
    file.close();

    showInfo(*this);
}

void showInfo(const Model &model) {
    std::cout << "-------------------------" << std::endl
              << "[Model Info]" << std::endl
              << "  Loaded object: " << model.name << std::endl
              << "  " << model.vertices.size() << " total vertices, " << model.indices.size() << " faces" << std::endl
              << "  hasUV: " << (model.hasUV ? "true" : "false") << std::endl
              << "-------------------------\n"
              << std::endl;
}

std::vector<Vertex> vts = {
    // +Z (front)
    {{-1,-1, 1}, {0,0,1}, {0,0}},
    {{ 1,-1, 1}, {0,0,1}, {0,0}},
    {{ 1, 1, 1}, {0,0,1}, {0,0}},
    {{-1, 1, 1}, {0,0,1}, {0,0}},

    // -Z (back)
    {{-1,-1,-1}, {0,0,-1}, {0,0}},
    {{ 1,-1,-1}, {0,0,-1}, {0,0}},
    {{ 1, 1,-1}, {0,0,-1}, {0,0}},
    {{-1, 1,-1}, {0,0,-1}, {0,0}},

    // +X (right)
    {{ 1,-1, 1}, {1,0,0}, {0,0}},
    {{ 1,-1,-1}, {1,0,0}, {0,0}},
    {{ 1, 1,-1}, {1,0,0}, {0,0}},
    {{ 1, 1, 1}, {1,0,0}, {0,0}},

    // -X (left)
    {{-1,-1,-1}, {-1,0,0}, {0,0}},
    {{-1,-1, 1}, {-1,0,0}, {0,0}},
    {{-1, 1, 1}, {-1,0,0}, {0,0}},
    {{-1, 1,-1}, {-1,0,0}, {0,0}},

    // +Y (top)
    {{-1, 1, 1}, {0,1,0}, {0,0}},
    {{ 1, 1, 1}, {0,1,0}, {0,0}},
    {{ 1, 1,-1}, {0,1,0}, {0,0}},
    {{-1, 1,-1}, {0,1,0}, {0,0}},

    // -Y (bottom)
    {{-1,-1,-1}, {0,-1,0}, {0,0}},
    {{ 1,-1,-1}, {0,-1,0}, {0,0}},
    {{ 1,-1, 1}, {0,-1,0}, {0,0}},
    {{-1,-1, 1}, {0,-1,0}, {0,0}},
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