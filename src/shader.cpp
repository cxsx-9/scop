#include "shader.hpp"

Shader::Shader(const char* vsPath, const char* fsPath) {
    std::string vsrc = readFile(vsPath);
    std::string fsrc = readFile(fsPath);
    GLuint vs = compile(GL_VERTEX_SHADER, vsrc);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fsrc);

    ID = glCreateProgram();
    glAttachShader(ID, vs);
    glAttachShader(ID, fs);
    glLinkProgram(ID);
    
    glValidateProgram(ID);

    GLint result;
    glGetProgramiv(ID, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        GLint len = 0;
        glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &len);
        std::string log; log.resize(len);
        glGetProgramInfoLog(ID, len, nullptr, &log[0]);
        std::cerr << "[Program Link Error] " << log << "\n";
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
}

void Shader::use() const {
     glUseProgram(ID); 
}

void Shader::setMat4(const std::string& name, const glm::mat4& m) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(m));
    // glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, m.value_ptr()); // for math.hpp
}
void Shader::setVec3(const std::string& name, const glm::vec3& v) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &v.x);
}
void Shader::setFloat(const std::string& name, float f) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), f);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

std::string Shader::readFile(const char* path) {
    std::ifstream file(path, std::ios::in);
    if(!file.is_open()){
        std::cerr << "[Error] Cannot open shader file: " << path << std::endl;
        return "";
    }
    std::stringstream ss; ss << file.rdbuf();
    return ss.str();
}

GLuint Shader::compile(GLenum type, const std::string&src) {
    GLuint id = glCreateShader(type);
    const char* s = src.c_str();
    glShaderSource(id, 1, &s, nullptr);
    glCompileShader(id);

    GLint result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        GLint len = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        std::string log; log.resize(len);
        glGetShaderInfoLog(id, len, &len, &log[0]);
        std::cerr << "[Shader Compile Error] " << log << "\n";
    }
    return id;
}