#pragma once

#include "scop.hpp"
#include "model.hpp"
#include "shader.hpp"

#include "inputManager.hpp"

class InputManager;

class Renderer {
public:
    InputManager& inputManager;

    Renderer(InputManager& inputManager) : inputManager(inputManager) {}

    void clear(float r=0.1f,float g=0.1f,float b=0.12f,float a=1.0f) {
        glClearColor(r,g,b,a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void draw(const Model& model, Shader& shader,
              const mymath::mat4& modelMatrix,
              const mymath::mat4& view,
              const mymath::mat4& proj)
    {
        shader.use();
        shader.setMat4("model", modelMatrix);
        shader.setMat4("view",  view);
        shader.setMat4("projection", proj);

        if (model.material.hasTexture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, model.material.textureID);
            shader.setInt("diffuseMap", 0);
        }
        shader.setInt("renderMode", inputManager.renderMode);
        shader.setInt("prevMode", inputManager.prevMode);
        shader.setFloat("mixFactor", inputManager.mixFactor);

        model.bind();
        glDrawElements(GL_TRIANGLES, (GLsizei)model.indices.size(), GL_UNSIGNED_INT, 0);
    }
};