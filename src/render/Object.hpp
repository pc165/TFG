#ifndef TFG_OBJECT_HPP
#define TFG_OBJECT_HPP

#include "Utils.h"
#include "Shader.h"

class Object {
public:
    explicit Object() {
        glGenVertexArrays(1, &vertexArrayId_);
        glCreateBuffers(1, &positionId_);
        glCreateBuffers(1, &normalId_);
        glCreateBuffers(1, &indexBufferId_);
        assert(vertexArrayId_ != 0);
        assert(positionId_ != 0);
        assert(indexBufferId_ != 0);
        assert(normalId_ != 0);

        LOG_INFO("Created object: va {} vp {} vn {} ib {}", vertexArrayId_, positionId_, normalId_, indexBufferId_);
    }

    ~Object() {
        glDeleteVertexArrays(1, &vertexArrayId_);
        glDeleteBuffers(1, &positionId_);
        glDeleteBuffers(1, &normalId_);
        glDeleteBuffers(1, &indexBufferId_);
    }

    void bindAll() const {
        glBindVertexArray(vertexArrayId_);
        glBindBuffer(GL_ARRAY_BUFFER, normalId_);
        glBindBuffer(GL_ARRAY_BUFFER, positionId_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);
    }

    void unBindAll() const {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void setupVao(std::vector<float> const &position, std::vector<float> const &normals, std::vector<unsigned int> const &index) {
        basicShader.loadSource("basicShader.glsl");
        glBindVertexArray(vertexArrayId_);

        // Indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned int), index.data(), GL_STATIC_DRAW);
        indexCount_ = index.size();

        // Position
        glBindBuffer(GL_ARRAY_BUFFER, positionId_);
        glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), position.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, (GLint) 3, GL_FLOAT, GL_TRUE, (GLint) 0, nullptr);

        // Normals
        glBindBuffer(GL_ARRAY_BUFFER, normalId_);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, (GLint) 3, GL_FLOAT, GL_FALSE, (GLint) 0, nullptr);
    }

    void bind() const {
        glBindVertexArray(vertexArrayId_);
    }

    virtual void draw(tfg::Transform const &data, glm::vec3 const &color) const {
        bind();
        basicShader.bind();

        auto translated = glm::translate(glm::mat4(1.0f), data.position);
        auto rotated = glm::rotate(translated, glm::radians(data.rotationDegress), data.rotationAxis);
        auto transform = glm::scale(rotated, data.scale);

        basicShader.setMat4("model", glm::value_ptr(transform));
        basicShader.setMat4("view", glm::value_ptr(Injector::camera->getViewMatrix()));
        basicShader.setMat4("projection", glm::value_ptr(Injector::camera->getProjectionMatrix()));
        basicShader.setVec3("colorIn", glm::value_ptr(color));
        glDrawElements(GL_TRIANGLES, (GLsizei) indexCount_, GL_UNSIGNED_INT, nullptr);
    }

protected:
    Shader basicShader{};
    uint32_t indexCount_{0};
    GLuint indexBufferId_{0};
    GLuint positionId_{0};
    GLuint normalId_{0};
    GLuint vertexArrayId_{0};
};

#endif //TFG_OBJECT_HPP
