#pragma once

#include "scop.hpp"
// #include "math.hpp"

class Shader {
    public:
        GLuint ID=0;
        Shader(const char* vsPath, const char* fsPath);
        void use() const;

        void setMat4(const std::string& name, const glm::mat4& m) const;
        void setVec3(const std::string& name, const glm::vec3& v) const;
        void setFloat(const std::string& name, float f) const;

    private:
        static std::string readFile(const char* path);
        static GLuint compile(GLenum type, const std::string& src);
};