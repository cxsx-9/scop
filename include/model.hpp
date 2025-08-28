#pragma once

#include "scop.hpp"

class Model{
    public:
        std::string name;
        GLuint VAO=0, VBO=0, EBO=0;
        
        std::vector<float> interleaved;
        std::vector<unsigned int> indices;
        GLsizei indexCount;

        void bind() const { glBindVertexArray(VAO); }

        Model();
        ~Model();

        void setup();
};