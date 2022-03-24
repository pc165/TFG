#ifndef TFG_CAMERA_H
#define TFG_CAMERA_H

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

class Camera {
public:
    Camera(float fovDegrees,
           float width, float height,
           float zNear = 0.01f, float zFar = 100.0f) {
        projectionMatrix_ = glm::perspective(glm::radians(fovDegrees), width / height, zNear, zFar);
        viewProjection_ = projectionMatrix_ * view_;
    };

    Camera(float left, float right, float bottom, float top) {
        projectionMatrix_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        viewProjection_ = projectionMatrix_ * view_;
    };

    void setPerspectiveProjection(float fovDegrees,
                                  float width, float height,
                                  float zNear = 0.01f, float zFar = 100.0f) {
        projectionMatrix_ = glm::perspective(glm::radians(fovDegrees), width / height, zNear, zFar);
        viewProjection_ = projectionMatrix_ * view_;
    };

    void setOrthographicProjection(float left, float right, float bottom, float top) {
        projectionMatrix_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        viewProjection_ = projectionMatrix_ * view_;
    };

    [[nodiscard]] const glm::vec3 &GetPosition() const { return position_; }

    void SetPosition(const glm::vec3 &position) {
        position_ = position;
        RecalculateViewMatrix();
    }

    [[nodiscard]] float getRotation() const { return rotationDegrees_; }

    void SetRotation(float rotation) {
        rotationDegrees_ = rotation;
        RecalculateViewMatrix();
    }

    [[nodiscard]] const glm::mat4 &getProjectionMatrix() const { return projectionMatrix_; }

    [[nodiscard]] const glm::mat4 &getViewMatrix() const { return view_; }

    [[nodiscard]] const glm::mat4 &getViewProjectionMatrix() const { return viewProjection_; }

    void setCenter(glm::vec3 &center) {
        center_ = center;
        RecalculateViewMatrix();
    }

    void setUp(glm::vec<3, int> &up) {
        up_ = up;
        RecalculateViewMatrix();
    }

    void RecalculateViewMatrix() {
        auto pos = glm::translate(glm::mat4(1.0f), position_);
        auto rot = glm::rotate(glm::mat4(1.0f), glm::radians(rotationDegrees_), center_);
        view_ = glm::inverse(pos * rot);
        view_ = glm::lookAt(
                position_,
                center_,
                up_
        );
        viewProjection_ = projectionMatrix_ * view_;
    };

private:
    glm::mat4 projectionMatrix_{};
    glm::mat4 view_{1.0f};
    glm::mat4 viewProjection_{};
    glm::vec3 position_{0};
    glm::vec3 center_{0, 0, 0};
    glm::vec3 up_{0, 1, 0};
    float rotationDegrees_ = 0.0f;
};

#endif //TFG_CAMERA_H
