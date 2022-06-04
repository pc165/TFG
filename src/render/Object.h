#ifndef TFG_OBJECT_H
#define TFG_OBJECT_H

#include "Utils.h"
#include "Shader.h"

class Object {
public:
    explicit Object() {
        glGenVertexArrays(1, &vertexArrayId_);
        glCreateBuffers(1, &positionId_);
        glCreateBuffers(1, &normalId_);
        glCreateBuffers(1, &textureId_);
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
        glDeleteBuffers(1, &textureId_);
        glDeleteBuffers(1, &indexBufferId_);
    }

    void bindAll() const {
        glBindVertexArray(vertexArrayId_);
        glBindBuffer(GL_ARRAY_BUFFER, normalId_);
        glBindBuffer(GL_ARRAY_BUFFER, textureId_);
        glBindBuffer(GL_ARRAY_BUFFER, positionId_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);
    }

//    void bindTextures() const {
//        // bind diffuse map
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, diffuseMap);
//
//        // bind specular map
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, specularMap);
//    }

    static void unBindAll() {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void setupVao(std::vector<float> const &position, std::vector<float> const &normals, std::vector<float> const &texture,
                  std::vector<unsigned int> const &index) {
        basicShader.loadSource("basicShader.glsl");
        normalShader.loadSource("normalShader.glsl");
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

        // texture
        glBindBuffer(GL_ARRAY_BUFFER, textureId_);
        glBufferData(GL_ARRAY_BUFFER, texture.size() * sizeof(float), texture.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, (GLint) 2, GL_FLOAT, GL_FALSE, (GLint) 0, nullptr);
    }

    void bind() const {
        glBindVertexArray(vertexArrayId_);
    }

    void draw(tfg::Transform const &data, glm::vec3 const &color, bool isLightEnabled, bool drawNormals = false) const {
        bind();
        basicShader.bind();

        auto translated = glm::translate(glm::mat4(1.0f), data.position);
        auto rotated = glm::rotate(translated, glm::radians(data.rotationDegress), data.rotationAxis);
        auto transform = glm::scale(rotated, data.scale);

        // vertex uniform
        basicShader.setMat4("uModel", glm::value_ptr(transform));
        basicShader.setMat4("uView", glm::value_ptr(GlobalOptions.camera->getViewMatrix()));
        basicShader.setMat4("uProjection", glm::value_ptr(GlobalOptions.camera->getProjectionMatrix()));

        // fragment uniform
        basicShader.setVec3("uColor", glm::value_ptr(color));
        basicShader.setVec3("uViewPos", glm::value_ptr(GlobalOptions.camera->position_));
        basicShader.setInt("uIsLightEnabled", isLightEnabled);

        basicShader.setMaterial(material);
        basicShader.setLight(GlobalOptions.light);

        glDrawElements(GL_TRIANGLES, (GLsizei) indexCount_, GL_UNSIGNED_INT, nullptr);

        if (drawNormals) {
            normalShader.bind();
            normalShader.setMat4("uModel", glm::value_ptr(transform));
            normalShader.setMat4("uView", glm::value_ptr(GlobalOptions.camera->getViewMatrix()));
            normalShader.setMat4("uProjection", glm::value_ptr(GlobalOptions.camera->getProjectionMatrix()));
            glDrawElements(GL_TRIANGLES, (GLsizei) indexCount_, GL_UNSIGNED_INT, nullptr);
        }
    }

//    void setDiffuseMap(glm::vec3 const &color) {
//        diffuseMap = tfg::colorToTexture(color, 32);
//        specularMap = tfg::colorToTexture(color, 32);
//    }


protected:
    Shader basicShader{};
    Shader normalShader{};
    uint32_t indexCount_{0};
    GLuint indexBufferId_{0};
    GLuint positionId_{0};
    GLuint normalId_{0};
    GLuint textureId_{0};
    GLuint vertexArrayId_{0};
//    GLuint diffuseMap{0};
//    GLuint specularMap{0};
    tfg::Material material;
};

#endif //TFG_OBJECT_H
