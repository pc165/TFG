#ifndef TFG_OBJECT_HPP
#define TFG_OBJECT_HPP

#define _USE_MATH_DEFINES

#include <cmath>
#include <glm/glm.hpp>
#include "Buffer.h"
#include "Shader.h"
#include "Camera.h"
#include "Tools.h"

class Object {
public:
    explicit Object() {
        basicShader.loadSource("basicShader.glsl");
    }

    virtual void draw(Transform const &data, glm::vec3 const &color) const {
        vao.bind();
        basicShader.bind();

        auto translated = glm::translate(glm::mat4(1.0f), data.position);
        auto rotated = glm::rotate(translated, glm::radians(data.rotationDegress), data.rotationAxis);
        auto transform = glm::scale(rotated, data.scale);

        basicShader.setMat4("model", glm::value_ptr(transform));
        basicShader.setMat4("view", glm::value_ptr(Tools::camera->getViewMatrix()));
        basicShader.setMat4("projection", glm::value_ptr(Tools::camera->getProjectionMatrix()));
        basicShader.setVec3("colorIn", glm::value_ptr(color));
        glDrawElements(GL_TRIANGLES, (GLsizei) ib.getCount(), GL_UNSIGNED_INT, nullptr);
    }

protected:
    Shader basicShader{};
    IndexBuffer<uint32_t> ib{};
    VertexBuffer vb{};
    VertexBuffer vn{};
    VertexArray vao{};
};

#endif //TFG_OBJECT_HPP
