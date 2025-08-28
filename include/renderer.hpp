#pragma once

#include "scop.hpp"
#include "model.hpp"
#include "shader.hpp"

class Renderer {
public:
    void clear(float r=0.1f,float g=0.1f,float b=0.12f,float a=1.0f) {
        glClearColor(r,g,b,a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void draw(const Model& meshModel, Shader& shader,
              const glm::mat4& model,
              const glm::mat4& view,
              const glm::mat4& proj,
              const glm::vec3& viewPos)
    {
        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("view",  view);
        shader.setMat4("projection", proj);
        shader.setVec3("viewPos", viewPos);

        meshModel.bind();
        glDrawElements(GL_TRIANGLES, meshModel.indexCount, GL_UNSIGNED_INT, 0); // with EBO
        // glDrawArrays(GL_TRIANGLES, 0, object.Triangles.size()); // with out EBO
    }
};