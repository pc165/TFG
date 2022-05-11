#ifndef TFG_CUBE_H
#define TFG_CUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdint>
#include "Buffer.h"
#include "Shader.h"
#include "Camera.h"
#include "Object.hpp"
#include <glm/gtc/type_ptr.hpp>

class Cube : public Object {
public:
    explicit Cube(Camera *camera) : Object(*camera) {
        std::vector<float> position = {
                1.0, -1.0, -1.0,
                1.0, -1.0, 1.0,
                -1.0, -1.0, 1.0,
                -1.0, -1.0, -1.0,
                1.0, 1.0, -1.0,
                1.0, 1.0, 1.0,
                -1.0, 1.0, 1.0,
                -1.0, 1.0, -1.0
        };
        std::vector<uint32_t> index = {
                4, 0, 3,
                4, 3, 7,
                2, 6, 7,
                2, 7, 3,
                1, 5, 2,
                5, 6, 2,
                0, 4, 1,
                4, 5, 1,
                4, 7, 5,
                7, 6, 5,
                0, 1, 2,
                0, 2, 3,
        };

        std::vector<float> normals = {
                0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,   // v0-v1-v2 (front)
                0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,   // v2-v3-v0
                1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,   // v0-v3-v4 (right)
                1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,   // v4-v5-v0
                0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,   // v0-v5-v6 (top)
                0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,   // v6-v1-v0
                -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,   // v1-v6-v7 (left)
                -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,   // v7-v2-v1
                0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,   // v7-v4-v3 (bottom)
                0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,   // v3-v2-v7
                0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0,   // v4-v7-v6 (back)
                0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0    // v6-v5-v4
        };

        shader.loadSource("cube.glsl");
        vao.bind();
        ib.create(index.data(), index.size());
        // Position
        vb.create(position.data(), position.size() * sizeof(float));
        vao.addLayout(vb, 0, VectorType::Vec3, Type::FLOAT);

        vn.create(normals.data(), normals.size() * sizeof(float));
        vao.addLayout(vn, 1, VectorType::Vec3, Type::FLOAT);
    }

    void draw() override {
        vao.bind();
        shader.bind();

        shader.setMat4("view", glm::value_ptr(camera_.getViewMatrix()));
        shader.setMat4("projection", glm::value_ptr(camera_.getProjectionMatrix()));

        for (auto i = 0; i < size; i++) {
            auto tra = glm::translate(glm::mat4(1.0f), position_[i]);
            auto rot = glm::rotate(tra, glm::radians(degrees_[i]), rotationAxis_[i]);
            model_[i] = glm::scale(rot, scale_[i]);
        }
        for (auto i = 0; i < size; i++) {
            shader.setMat4("model", glm::value_ptr(model_[i]));
            shader.setVec3("colorIn", glm::value_ptr(color_[i]));
            glDrawElements(GL_TRIANGLES, (GLsizei) ib.getCount(), GL_UNSIGNED_INT, nullptr);
        }
    }

    void drawPickObject() override {
        vao.bind();
        shader.bind();

        shader.setMat4("view", glm::value_ptr(camera_.getViewMatrix()));
        shader.setMat4("projection", glm::value_ptr(camera_.getProjectionMatrix()));

        for (auto i = 0; i < size; i++) {
            auto tra = glm::translate(glm::mat4(1.0f), position_[i]);
            auto rot = glm::rotate(tra, glm::radians(degrees_[i]), rotationAxis_[i]);
            model_[i] = glm::scale(rot, scale_[i]);
        }

        for (auto i = 0; i < size; i++) {
            shader.setMat4("model", glm::value_ptr(model_[i]));
            shader.setVec3("colorIn", glm::value_ptr(pickColor_[i]));
            glDrawElements(GL_TRIANGLES, (GLsizei) ib.getCount(), GL_UNSIGNED_INT, nullptr);
        }
    }
};

#endif //TFG_CUBE_H
