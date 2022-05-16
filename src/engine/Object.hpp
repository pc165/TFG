#ifndef TFG_OBJECT_HPP
#define TFG_OBJECT_HPP

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdint>
#include "Buffer.h"
#include "Shader.h"
#include "Camera.h"
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/type_ptr.hpp>

class Object {

public:
    explicit Object(Camera &camera, bool enablePick = true) : enablePick_(enablePick), camera_(camera) {}

    void add(const glm::vec3 &pos = glm::vec3(0, 0, 0),
             const glm::vec3 &scale = glm::vec3(1, 1, 1),
             const glm::vec3 &rotAxis = glm::vec3(1, 0, 0),
             const glm::vec3 &color = glm::vec3(1, 0, 0),
             float degrees = 0) {
        position_.push_back(pos);
        rotationAxis_.push_back(rotAxis);
        scale_.push_back(scale);
        degrees_.push_back(degrees);

        color_.emplace_back(color);
        pickColor_.emplace_back(enablePick_ ? genPickColor() : glm::vec3(1, 1, 1));

        model_.emplace_back();
        size++;
    }

    void remove(int i) {
        assert(i < size);
        position_.erase(position_.begin() + i);
        rotationAxis_.erase(rotationAxis_.begin() + i);
        scale_.erase(scale_.begin() + i);
        degrees_.erase(degrees_.begin() + i);

        color_.erase(color_.begin() + i);
        pickColor_.erase(pickColor_.begin() + i);

        model_.erase(model_.begin() + i);
        size--;
    }

    virtual void draw() = 0;

    virtual void drawPickObject() = 0;

    static int colorToId(glm::vec3 color) {
        if (color == glm::vec3(1, 1, 1))
            return -1;

        int id = std::round(color.r * 10) +
                 std::round(color.g * 100) +
                 std::round(color.b * 1000);
        return id;
    };

private:
    [[nodiscard]] glm::vec3 genPickColor() const {
        int n = pickColor_.size();
        float r = int((n % 10)) / 10.f;
        float g = int((n % 100) / 10) / 10.f;
        float b = int((n % 1000) / 100) / 100.f;
        glm::vec3 newColor(r, g, b);
        LOG_DEBUG("Pick color {}", glm::to_string(newColor).c_str());
        return newColor;
    }

private:
    bool enablePick_ = true;

protected:
    Shader shader{};
    IndexBuffer<uint32_t> ib{};
    VertexBuffer vb{};
    VertexBuffer vn{};
    VertexArray vao{};

public:
    std::vector<glm::vec3> position_{};
    std::vector<glm::vec3> rotationAxis_{};
    std::vector<glm::vec3> scale_{};
    std::vector<float> degrees_{};

    std::vector<glm::vec3> color_{};
    std::vector<glm::vec3> pickColor_{};

    std::vector<glm::mat4> model_{};
    int size{0};
    Camera &camera_;
};

#endif //TFG_OBJECT_HPP
