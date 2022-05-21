#ifndef TFG_CAMERA_H
#define TFG_CAMERA_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "Event.h"

class Camera {
public:
    glm::vec3 pos, center, up;
    glm::vec3 right, worldUp;
    float yaw{270}, pitch{0};
    float movementSpeed{4};
    float mouseSensitivity{0.1};
    float fov{45};
    float width{0}, height{1};
    float zNear{0.01f}, zFar{100.0f};

    explicit Camera(glm::vec3 position = glm::vec3(0, 0, 0),
                    glm::vec3 up = glm::vec3(0, 1, 0),
                    float width = 1080,
                    float height = 720) :
            pos(position), center(), up(),
            right(), worldUp(up),
            width(width), height(height) {
        updateCameraVectors();
    }

    [[nodiscard]] auto getViewMatrix() const {
        return glm::lookAt(pos, pos + center, up);
    }

    [[nodiscard]] auto getProjectionMatrix() const {
        return glm::perspective(glm::radians(fov), width / height, zNear, zFar);
    };

    [[nodiscard]] auto getOrthoMatrix() const {
        return glm::ortho(0.0f, width, height, 0.0f, 0.0f, 1.0f);
    }

    bool onEvent(const Event &event, double deltaTime) {
        switch (event.type) {
            case WindowResize: {
                auto window = dynamic_cast<const WindowResizeEvent *>(&event);
                width = window->width;
                height = window->height;
                break;
            }
            case Key: {
                auto key = dynamic_cast<const KeyEvent *>(&event);
                float velocity = movementSpeed * deltaTime;
                switch (key->key) {
                    case GLFW_KEY_W:
                        pos += center * velocity;
                        break;
                    case GLFW_KEY_S:
                        pos -= center * velocity;
                        break;
                    case GLFW_KEY_A:
                        pos -= right * velocity;
                        break;
                    case GLFW_KEY_D:
                        pos += right * velocity;
                        break;
                    case GLFW_KEY_SPACE:
                        pos.y += velocity;
                        break;
                    case GLFW_KEY_LEFT_CONTROL:
                        pos.y -= velocity;
                        break;
                }
                break;
            }
            case MouseMoved: {
                auto mouse = dynamic_cast<const MouseMoveEvent *>(&event);
                if (!buttonPress && !isFreeCamera)
                    break;

                if (firstMove) {
                    lastX = mouse->xPos;
                    lastY = mouse->yPos;
                    firstMove = false;
                }

                auto xoffset = mouse->xPos - lastX;
                auto yoffset = lastY - mouse->yPos;

                lastX = mouse->xPos;
                lastY = mouse->yPos;

                xoffset *= mouseSensitivity;
                yoffset *= mouseSensitivity;

                yaw += xoffset;
                pitch += yoffset;

                if (yaw > 360.0f)
                    yaw = 0.0f;
                if (yaw < 0)
                    yaw = 360.f;

                if (pitch > 89.0f)
                    pitch = 89.0f;
                if (pitch < -89.0f)
                    pitch = -89.0f;

                updateCameraVectors();
                break;
            }
            case MouseScrolled: {
                auto mouse = dynamic_cast<const MouseScrollEvent *>(&event);
                fov -= (float) mouse->yOffset;
                if (fov < 1.0f)
                    fov = 1.0f;
                if (fov > 179.0f) {
                    fov = 179.0f;
                }
                break;
            }
            case MouseButton: {
                auto mouse = dynamic_cast<const MouseButtonEvent *>(&event);
                buttonPress = mouse->press_release == 0 && !isFreeCamera;
                if (mouse->press_release == 1)
                    firstMove = true;
                break;
            }
            default:
                break;
        }
        return false;
    }

    void setFreeCamera(bool enabled) {
        isFreeCamera = enabled;
    }

    void setFirstMove() {
        firstMove = true;
    }

    void updateCameraVectors() {
        glm::vec3 front2;
        front2.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front2.y = sin(glm::radians(pitch));
        front2.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        center = glm::normalize(front2);
        right = glm::normalize(glm::cross(center, worldUp));
        up = glm::normalize(glm::cross(right, center));
    }

private:
    float lastX{0}, lastY{0};
    bool firstMove{true}, buttonPress{false};
    bool isFreeCamera{false};
};

#endif //TFG_CAMERA_H
