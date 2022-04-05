#ifndef TFG_CUBE_H
#define TFG_CUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdint>
#include "Buffer.h"
#include "Shader.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>

class Cube {
public:
    explicit Cube(Camera *camera) : camera_(*camera) {
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
                5, 1, 4,
                5, 4, 8,
                3, 7, 8,
                3, 8, 4,
                2, 6, 3,
                6, 7, 3,
                1, 5, 2,
                5, 6, 2,
                5, 8, 6,
                8, 7, 6,
                1, 2, 3,
                1, 3, 4,
        };
        for (auto &i: index) {
            i -= 1;
        }
        shader.loadSource("cube.glsl");
        vao.bind();
        ib.create(index.data(), index.size());
        vb.create(position.data(), position.size() * sizeof(float));
        vao.addLayout(vb, 0, VectorType::Vec3, Type::FLOAT);
        vao.addLayout(vb, 1, VectorType::Vec3, Type::FLOAT);
    }

    void draw() {
        vao.bind();
        shader.bind();
        shader.setMat4("view", glm::value_ptr(camera_.getViewMatrix()));
        shader.setMat4("projection", glm::value_ptr(camera_.getProjectionMatrix()));
        for (auto i = 0; i < size; i++) {
            auto Model = glm::translate(glm::mat4(1.0f), position_[i]);
            Model = glm::rotate(Model, glm::radians(degrees_[i]), rotationAxis_[i]);
            Model = glm::scale(Model, scale_[i]);
            shader.setMat4("model", glm::value_ptr(Model));
            glDrawElements(GL_TRIANGLES, (GLsizei) ib.getCount(), GL_UNSIGNED_INT, nullptr);
        }
    }

    void add(glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 rotAxis = glm::vec3(1, 0, 0),
             glm::vec3 scale = glm::vec3(1, 1, 1), float degrees = 0) {
        position_.push_back(pos);
        rotationAxis_.push_back(rotAxis);
        scale_.push_back(scale);
        degrees_.push_back(degrees);
        size++;
    }

    void remove(int i) {
        assert(i < size);
        position_.erase(position_.begin() + i);
        rotationAxis_.erase(rotationAxis_.begin() + i);
        scale_.erase(scale_.begin() + i);
        degrees_.erase(degrees_.begin() + i);
        size--;
    }

private:
    Shader shader{};
    IndexBuffer<uint32_t> ib{};
    VertexBuffer vb{};
    VertexBuffer vc{};
    VertexArray vao{};

public:
    std::vector<glm::vec3> position_{};
    std::vector<glm::vec3> rotationAxis_{};
    std::vector<glm::vec3> scale_{};
    std::vector<float> degrees_{};
    int size{0};
    Camera &camera_;
};


#endif //TFG_CUBE_H
