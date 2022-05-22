#ifndef TFG_CAMERA_H
#define TFG_CAMERA_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "Event.h"
#include "Logger.h"

class Camera {
public:
    glm::vec3 position_, center_, up_;
    glm::vec3 right, worldUp_;
    float yaw_{270}, pitch_{0};
    float movementSpeed_{4};
    float mouseSensitivity_{0.1};
    float fov_{45};
    float width_{0}, height_{1};
    float zNear_{0.001f}, zFar_{100.0f};

    explicit Camera(glm::vec3 position = glm::vec3(0, 0, 0),
                    glm::vec3 up = glm::vec3(0, 1, 0),
                    float width = 1080,
                    float height = 720) :
            position_(position), center_(), up_(),
            right(), worldUp_(up),
            width_(width), height_(height) {
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

    bool onEvent(const Event &event, double deltaTime) {
        switch (event.type) {
            case WindowResize: {
                auto window = dynamic_cast<const WindowResizeEvent *>(&event);
                width_ = window->width;
                height_ = window->height;
                break;
            }
            case Key: {
                auto key = dynamic_cast<const KeyEvent *>(&event);
                float velocity = movementSpeed_ * deltaTime;
                switch (key->key) {
                    case GLFW_KEY_W:
                        position_ += center_ * velocity;
                        break;
                    case GLFW_KEY_S:
                        position_ -= center_ * velocity;
                        break;
                    case GLFW_KEY_A:
                        position_ -= right * velocity;
                        break;
                    case GLFW_KEY_D:
                        position_ += right * velocity;
                        break;
                    case GLFW_KEY_SPACE:
                        position_.y += velocity;
                        break;
                    case GLFW_KEY_LEFT_CONTROL:
                        position_.y -= velocity;
                        break;
                }
                break;
            }
            case MouseMoved: {
                auto mouse = dynamic_cast<const MouseMoveEvent *>(&event);
                if (!buttonPress_ && !isFreeCamera_)
                    break;

                if (firstMove_) {
                    lastX_ = mouse->xPos;
                    lastY_ = mouse->yPos;
                    firstMove_ = false;
                }

                auto xoffset = mouse->xPos - lastX_;
                auto yoffset = lastY_ - mouse->yPos;

                lastX_ = mouse->xPos;
                lastY_ = mouse->yPos;

                xoffset *= mouseSensitivity_;
                yoffset *= mouseSensitivity_;

                yaw_ += xoffset;
                pitch_ += yoffset;

                if (yaw_ > 360.0f)
                    yaw_ = 0.0f;
                if (yaw_ < 0)
                    yaw_ = 360.f;

                if (pitch_ > 89.0f)
                    pitch_ = 89.0f;
                if (pitch_ < -89.0f)
                    pitch_ = -89.0f;

                updateCameraVectors();
                break;
            }
            case MouseScrolled: {
                auto mouse = dynamic_cast<const MouseScrollEvent *>(&event);
                fov_ -= (float) mouse->yOffset;
                if (fov_ < 1.0f)
                    fov_ = 1.0f;
                if (fov_ > 179.0f) {
                    fov_ = 179.0f;
                }
                break;
            }
            case MouseButton: {
                auto mouse = dynamic_cast<const MouseButtonEvent *>(&event);
                buttonPress_ = mouse->press_release == 0 && !isFreeCamera_;
                if (mouse->press_release == 1)
                    firstMove_ = true;
                break;
            }
            default:
                break;
        }
        return false;
    }

    void setFreeCamera(bool enabled) {
        isFreeCamera_ = enabled;
    }

    void setFirstMove() {
        firstMove_ = true;
    }

    void updateCameraVectors() {
        glm::vec3 front2;
        front2.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front2.y = sin(glm::radians(pitch_));
        front2.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        center_ = glm::normalize(front2);
        right = glm::normalize(glm::cross(center_, worldUp_));
        up_ = glm::normalize(glm::cross(right, center_));
    }

private:
    float lastX_{0}, lastY_{0};
    bool firstMove_{true}, buttonPress_{false};
    bool isFreeCamera_{false};
};

#endif //TFG_CAMERA_H
