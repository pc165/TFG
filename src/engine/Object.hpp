#ifndef TFG_OBJECT_HPP
#define TFG_OBJECT_HPP

#include <cmath>
#include <glm/glm.hpp>
#include <cstdint>
#include "Buffer.h"
#include "Shader.h"
#include "Camera.h"
#include "Tools.h"
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
        pickColor_.emplace_back(enablePick_ ? Tools::genPickColor(pickColor_.size()) : glm::vec3(1, 1, 1));

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
