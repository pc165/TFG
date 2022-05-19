#ifndef TFG_OBJECT_HPP
#define TFG_OBJECT_HPP

#include <glm/glm.hpp>
#include "Buffer.h"
#include "Shader.h"
#include "Camera.h"
#include "Tools.h"

class Object {
public:
    explicit Object() : camera_(Tools::camera) {
        assert(camera_ != nullptr);
        basicShader.loadSource("basicShader.glsl");
    }

    virtual void draw() const {
        vao.bind();
        basicShader.bind();
        basicShader.setMat4("view", glm::value_ptr(camera_->getViewMatrix()));
        basicShader.setMat4("projection", glm::value_ptr(camera_->getProjectionMatrix()));

        for (auto i = 0; i < size; i++) {
            auto model = getProductModel(i);
            basicShader.setMat4("model", glm::value_ptr(model));
            basicShader.setVec3("colorIn", glm::value_ptr(color_[i]));
            glDrawElements(GL_TRIANGLES, (GLsizei) ib.getCount(), GL_UNSIGNED_INT, nullptr);
        }
    }

    virtual void drawPickObject() const {
        vao.bind();
        basicShader.bind();

        basicShader.setMat4("view", glm::value_ptr(camera_->getViewMatrix()));
        basicShader.setMat4("projection", glm::value_ptr(camera_->getProjectionMatrix()));

        for (auto i = 0; i < size; i++) {
            auto model = getProductModel(i);
            basicShader.setMat4("model", glm::value_ptr(model));
            basicShader.setVec3("colorIn", glm::value_ptr(pickColor_[i]));
            glDrawElements(GL_TRIANGLES, (GLsizei) ib.getCount(), GL_UNSIGNED_INT, nullptr);
        }
    }

protected:
    Shader basicShader{};
    IndexBuffer<uint32_t> ib{};
    VertexBuffer vb{};
    VertexBuffer vn{};
    VertexArray vao{};

protected:
    std::vector<glm::vec3> position_{};
    std::vector<glm::vec3> rotationAxis_{};
    std::vector<glm::vec3> scale_{};
    std::vector<float> degrees_{};

    std::vector<glm::vec3> color_{};
    std::vector<glm::vec3> pickColor_{};

    std::vector<glm::mat4> model_{};
    int size{0};
    Camera *camera_;

    // getters and setters
public:
    [[nodiscard]] glm::mat4 getProductModel(ObjectData const &data) const {
        auto translated = glm::translate(glm::mat4(1.0f), data.position);
        auto rotated = glm::rotate(translated, glm::radians(data.rotationDegress), data.rotationAxis);
        auto scaled = glm::scale(rotated, data.scale);
        return scaled;
    }

    [[nodiscard]] const std::vector<glm::vec3> &getPosition() const {
        return position_;
    }

    void setPosition(const std::vector<glm::vec3> &position) {
        position_ = position;
    }

    [[nodiscard]] const std::vector<glm::vec3> &getRotationAxis() const {
        return rotationAxis_;
    }

    void setRotationAxis(const std::vector<glm::vec3> &rotationAxis) {
        rotationAxis_ = rotationAxis;
    }

    [[nodiscard]] const std::vector<glm::vec3> &getScale() const {
        return scale_;
    }

    void setScale(const std::vector<glm::vec3> &scale) {
        scale_ = scale;
    }

    [[nodiscard]] const std::vector<float> &getDegrees() const {
        return degrees_;
    }

    void setDegrees(const std::vector<float> &degrees) {
        degrees_ = degrees;
    }

    [[nodiscard]] const std::vector<glm::vec3> &getColor() const {
        return color_;
    }

    void setColor(const std::vector<glm::vec3> &color) {
        color_ = color;
    }

    [[nodiscard]] const std::vector<glm::vec3> &getPickColor() const {
        return pickColor_;
    }

    void setPickColor(const std::vector<glm::vec3> &pickColor) {
        pickColor_ = pickColor;
    }

    [[nodiscard]] const std::vector<glm::mat4> &getModel() const {
        return model_;
    }

    void setModel(const std::vector<glm::mat4> &model) {
        model_ = model;
    }

    [[nodiscard]] int getSize() const {
        return size;
    }
};

#endif //TFG_OBJECT_HPP
