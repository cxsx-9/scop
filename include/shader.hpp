#pragma once

#include "scop.hpp"
// #include "math.hpp"

class Shader {
    public:
        GLuint ID=0;
        Shader(const char* vsPath, const char* fsPath);
        void use() const;

        void setMat4(const std::string& name, const mymath::mat4& m) const;
        void setVec3(const std::string& name, const mymath::vec3& v) const;
        void setFloat(const std::string& name, float f) const;
        
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;

    private:
        static std::string readFile(const char* path);
        static GLuint compile(GLenum type, const std::string& src);
};