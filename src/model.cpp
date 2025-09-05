#include "model.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "math.hpp"

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

    if (!material.hasTexture) return;
    
    // setup material if has
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(material.path.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "[Error] Failed to load texture: " << material.path << std::endl;
        return;
    }

    GLenum format = (channels == 1 ? GL_RED : (channels == 3 ? GL_RGB : GL_RGBA));

    glGenTextures(1, &material.diffuseMap);
    glBindTexture(GL_TEXTURE_2D, material.diffuseMap);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
};

void Model::bind() const { 
    glBindVertexArray(VAO);
}

void Model::centerAndNormalize(float targetExtent = 2.0f, float margin = 0.9f) {
    if (vertices.empty()) return;

    mymath::vec3 minp(FLT_MAX);
    mymath::vec3 maxp(-FLT_MAX);

    for (auto &v : vertices) {
        minp = mymath::min(minp, v.position);
        maxp = mymath::max(maxp, v.position);
    }

    mymath::vec3 center = (minp + maxp) * 0.5f;
    mymath::vec3 size   = maxp - minp;
    float maxExtent  = std::max(size.x, std::max(size.y, size.z));

    float scale = (targetExtent / maxExtent) * margin;

    for (auto &v : vertices) {
        v.position = (v.position - center) * scale;
    }

    for (auto &v : vertices) {
        v.normal = mymath::normalize(v.normal);
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

        mymath::vec3 edge1 = v1.position - v0.position;
        mymath::vec3 edge2 = v2.position - v0.position;
        mymath::vec3 faceNormal = mymath::normalize(mymath::cross(edge1, edge2));

        v0.normal += faceNormal;
        v1.normal += faceNormal;
        v2.normal += faceNormal;
    }
    for (auto &v : this->vertices) {
        v.normal = mymath::normalize(v.normal);
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

void computeBounds(const std::vector<Vertex>& vertices, mymath::vec3 &minp, mymath::vec3 &maxp) {
    if (vertices.empty()) {
        minp = mymath::vec3(0.0f);
        maxp = mymath::vec3(0.0f);
        return;
    }
    minp = mymath::vec3(FLT_MAX);
    maxp = mymath::vec3(-FLT_MAX);
    for (const auto &v : vertices) {
        minp = mymath::min(minp, v.position);
        maxp = mymath::max(maxp, v.position);
    }
}

void Model::generateAutoUVs() {
    if (vertices.empty()) return;

    mymath::vec3 minp, maxp;
    computeBounds(vertices, minp, maxp);
    mymath::vec3 ext = maxp - minp; // size of the bounding box

    if (ext.x < mymath::EPS) ext.x = mymath::EPS;
    if (ext.y < mymath::EPS) ext.y = mymath::EPS;
    if (ext.z < mymath::EPS) ext.z = mymath::EPS;

    for (auto &v : vertices) {
        mymath::vec3 n = mymath::abs(v.normal);
        float ax = n.x, ay = n.y, az = n.z;
        if (ax + ay + az < 1e-6f) {
            mymath::vec3 ap = mymath::abs(v.position);
            ax = ap.x; ay = ap.y; az = ap.z;
        }

        mymath::vec2 uv(0.0f, 0.0f);

        if (ax >= ay && ax >= az) {
            // project onto YZ plane (using z -> u, y -> v)
            uv.x = (v.position.z - minp.z) / ext.z;
            uv.y = (v.position.y - minp.y) / ext.y;
        } else if (ay >= ax && ay >= az) {
            // project onto XZ plane (x -> u, z -> v)
            uv.x = (v.position.x - minp.x) / ext.x;
            uv.y = (v.position.z - minp.z) / ext.z;
        } else {
            // az largest -> project onto XY plane (x -> u, y -> v)
            uv.x = (v.position.x - minp.x) / ext.x;
            uv.y = (v.position.y - minp.y) / ext.y;
        }
        v.texCoord = uv;
    }
    this->hasUV = true;
}

void Model::loadTexture(const std::string &path) {
    material.path = path;
    material.hasTexture = true;
    if (!this->hasUV) generateAutoUVs();
}

void Model::loadObject(const std::string &path) {
     std::ifstream file(path, std::ios::in);
    if(path.empty() || !file.is_open()) {
        std::cerr << "[Error] Cannot open object file from : '" << path << "'" << std::endl;
        mockup(*this);
        return;
    }

    std::vector<mymath::vec3> positions;
    std::vector<mymath::vec2> texcoords;
    std::vector<mymath::vec3> normals;

    std::string line;
    while (std::getline(file, line)) {
        std::string prefix;
        std::istringstream stream(line);
        stream >> prefix;

        if (line.empty() || prefix == "#") continue;

        if (prefix == "o") {
            stream >> this->name;
        } else if (prefix == "v") {
            mymath::vec3 pos;
            stream >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        } else if (prefix == "vt") {
            mymath::vec2 uv;
            stream >> uv.x >> uv.y;
            texcoords.push_back(uv);
        } else if (prefix == "vn") {
            mymath::vec3 norm;
            stream >> norm.x >> norm.y >> norm.z;
            normals.push_back(norm);
        } else if (prefix == "f") {
            std::vector<unsigned int> faceIndices;
            std::string token;
            while (stream >> token) {
                try {
                    FaceIndex idx = parseFaceToken(token);
                    Vertex vertex;
                    vertex.position = positions[idx.v];
                    vertex.texCoord = (idx.t >= 0 && idx.t < (int)texcoords.size()) ? texcoords[idx.t] : mymath::vec2(0.0f);
                    vertex.normal   = (idx.n >= 0 && idx.n < (int)normals.size()) ? normals[idx.n] : mymath::vec3(0.0f);

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
}

void Model::showInfo() const {
    std::cout << "-----------------------------------" << std::endl
              << "[Object Info]" << std::endl
              << "  Loaded object: " << this->name << std::endl
              << "  " << this->vertices.size() << " total vertices, " << this->indices.size() << " faces" << std::endl
              << "  hasUV: " << (this->hasUV ? "true" : "false") << std::endl
              << "  Material: " << (this->material.hasTexture ? ("texture '" + this->material.path + "'") : "default color") << std::endl
              << "-----------------------------------" << std::endl
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
