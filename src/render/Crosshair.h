#ifndef TFG_CROSSHAIR_H
#define TFG_CROSSHAIR_H

#include "render/Object.h"

class Crosshair : public Object {
public:
    glm::vec3 scale_{10};
    glm::vec3 color_{1, 1, 0};


    Crosshair() {
        std::vector<float> position = {
                -1, 0.15,
                1, 0.15,
                -1, -0.15,
                1, -0.15,

                -0.15, 1,
                0.15, 1,
                0.15, -1,
                -0.15, -1
        };
        std::vector<unsigned int> indices = {
                0, 1, 2,
                2, 1, 3,

                4, 5, 6,
                6, 7, 4
        };

        setupVao(position, indices);
    }

    void setupVao(std::vector<float> const &position, std::vector<unsigned int> const &index) {
        basicShader.loadSource("crosshair.glsl");
        glBindVertexArray(vertexArrayId_);

        // Indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned int), index.data(), GL_STATIC_DRAW);
        indexCount_ = index.size();

        // Position
        glBindBuffer(GL_ARRAY_BUFFER, positionId_);
        glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), position.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, (GLint) 2, GL_FLOAT, GL_FALSE, (GLint) 0, nullptr);
    }

    void draw() const {
        bind();
        basicShader.bind();

        auto translated = glm::translate(glm::mat4(1.0f), {GlobalOptions.camera->width_ / 2, GlobalOptions.camera->height_ / 2, 0});
        auto model = glm::scale(translated, scale_);

        basicShader.setMat4("model", glm::value_ptr(model));
        basicShader.setMat4("projection", glm::value_ptr(GlobalOptions.camera->getOrthoMatrix()));
        basicShader.setVec3("colorIn", glm::value_ptr(color_));
        glDrawElements(GL_TRIANGLES, (GLsizei) indexCount_, GL_UNSIGNED_INT, nullptr);
    };
};

#endif //TFG_CROSSHAIR_H
