#ifndef TFG_CAMERA_H
#define TFG_CAMERA_H

#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "Logger.h"
#include "DataStructs.h"

class Camera {
public:
    glm::vec3 position_, center_, up_;
    glm::vec3 right, worldUp_;
    float yaw_{270}, pitch_{0};
    float movementSpeed_{10};
    float mouseSensitivity_{0.1};
    float fov_{45};
    float width_{0}, height_{1};
    float zNear_{0.001f}, zFar_{100.0f};

    explicit Camera(EventState *eventState, glm::vec3 position = glm::vec3(0, 0, 0),
                    glm::vec3 up = glm::vec3(0, 1, 0),
                    float width = 1080,
                    float height = 720) : position_(position),
                                          center_(), up_(), right(),
                                          worldUp_(up), width_(width),
                                          height_(height), eventState_(eventState) {
        assert(eventState_ != nullptr);
        updateCameraVectors();
    }

    [[nodiscard]] auto getViewMatrix() const {
        return glm::lookAt(position_, position_ + center_, up_);
    }

    [[nodiscard]] auto getProjectionMatrix() const {
        return glm::perspective(glm::radians(fov_), width_ / height_, zNear_, zFar_);
    };

    [[nodiscard]] auto getOrthoMatrix() const {
        return glm::ortho(0.0f, width_, height_, 0.0f, -1.0f, 1.0f);
    }

    bool onUpdate(double deltaTime) {
        float velocity = movementSpeed_ * deltaTime;
        if (eventState_->keyDown(GLFW_KEY_W))
            position_ += center_ * velocity;

        if (eventState_->keyDown(GLFW_KEY_S))
            position_ -= center_ * velocity;

        if (eventState_->keyDown(GLFW_KEY_A))
            position_ -= right * velocity;

        if (eventState_->keyDown(GLFW_KEY_D))
            position_ += right * velocity;

        if (eventState_->keyDown(GLFW_KEY_SPACE))
            position_.y += velocity;

        if (eventState_->keyDown(GLFW_KEY_LEFT_CONTROL))
            position_.y -= velocity;

        if (eventState_->isMouseScrolled()) {
            fov_ -= (float) eventState_->getMouseScroll().y;
            if (fov_ < 1.0f)
                fov_ = 1.0f;
            if (fov_ > 179.0f)
                fov_ = 179.0f;
        }

        if (eventState_->isWindowSizeChanged()) {
            auto window = eventState_->getWindowSize();
            width_ = window.x;
            height_ = window.y;
        }

        if (!isFreeCamera_ && eventState_->isMouseMoved() && !eventState_->mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
            lastMousePos_ = eventState_->getMousePosition();
        }

        auto moveCamera = eventState_->isMouseMoved() && eventState_->mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT);

        if (isFreeCamera_ || moveCamera) {
            auto mouse = eventState_->getMousePosition();

            yaw_ += (mouse.x - lastMousePos_.x) * mouseSensitivity_;
            pitch_ += (lastMousePos_.y - mouse.y) * mouseSensitivity_;

            if (yaw_ > 360.0f)
                yaw_ = 0.0f;
            if (yaw_ < 0)
                yaw_ = 360.f;

            if (pitch_ > 89.0f)
                pitch_ = 89.0f;
            if (pitch_ < -89.0f)
                pitch_ = -89.0f;

            lastMousePos_ = mouse;

            updateCameraVectors();
            return true;
        }
        return false;
    }

    void setFreeCamera(bool enabled) {
        isFreeCamera_ = enabled;
    }

    void updateCameraVectors() {
        glm::vec3 front2;
        front2.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front2.y = sin(glm::radians(pitch_));
        front2.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        center_ = glm::normalize(front2);
        right = glm::normalize(glm::cross(center_, worldUp_));
        up_ = glm::normalize(glm::cross(right, center_));

        // update plane
        auto pos = position_;
        pos.z -= 1;
        plane_.setNormalVector(position_ - center_);
        plane_.movePlaneToPoint(pos);
    }

private:
    EventState *eventState_{nullptr};
    glm::vec2 lastMousePos_{0};
    bool isFreeCamera_{false};
    tfg::Plane plane_;
};

#endif //TFG_CAMERA_H
