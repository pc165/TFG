#ifndef TFG_WORLD_H
#define TFG_WORLD_H

#include "GameGui.h"
#include "Tile.h"
#include "Event.h"
#include "Tools.h"
#include <cmath>
#include <glm/gtx/string_cast.hpp>


class World {
public:
    explicit World() {
        Tools::camera = &this->camera_;
        assert(Tools::camera != nullptr);
        assert(Tools::window != nullptr);
        assert(Tools::windowStruct != nullptr);
        int number = 0;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                board_.addTile({j, i, 0}, number % 10);
                number++;
            }
        }
    }

    void gameLoop() {
        auto windowProps = Tools::windowStruct;
        windowProps->eventCallback = [this](const Event &event) {
            if (!onEvent(event))
                camera_.onEvent(event, deltaFrmame_);
        };

        double t0 = 0;
        while (!windowProps->shouldClose) {
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            board_.drawBoard(true);
            glfwPollEvents();
            glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            auto t1 = glfwGetTime();
            deltaFrmame_ = t1 - t0;
            board_.drawBoard();
            GameGui::drawwGUI([this]() {
                guiOverlay();
            }, [this]() {
                guiWindow();
            });
            t0 = t1;
            glfwSwapBuffers(Tools::window);
        };
    };

    void guiOverlay() {
        ImGui::Text("S2W: (%s)", glm::to_string(screentoWorldPos_).c_str());
        ImGui::Text("S2C: (%s)", glm::to_string(screenColor_).c_str());
        ImGui::Text("Hover entity id: (%d)", entityId_);
        ImGui::Text("Selected entity id: (%d)", selectedEntityId_);
        ImGui::Text("Mouse button: (%d)", buttonPress);
    }

    void guiWindow() {
        ImGui::Begin("Controls");
        ImGui::SliderFloat3("Position", glm::value_ptr(camera_.pos), -10, 10);
        ImGui::SliderFloat3("Center", glm::value_ptr(camera_.center), -10, 10);
        ImGui::SliderFloat3("Up", glm::value_ptr(camera_.up), -1, 1);
        ImGui::Separator();
        ImGui::End();
    }

    bool onEvent(const Event &event) {
        switch (event.type) {
            case Key: {
                auto key = dynamic_cast<const KeyEvent *>(&event);
                if (key->key == GLFW_KEY_2 && key->press_release_repeat == 1) {
                    Tools::setFreeCamera(false);
                    return true;
                }
                if (key->key == GLFW_KEY_1 && key->press_release_repeat == 1) {
                    Tools::setFreeCamera(true);
                    return true;
                }
                break;
            }
            case MouseMoved: {
                auto mouse = dynamic_cast<const MouseMoveEvent *>(&event);
                screentoWorldPos_ = Tools::screenToWorld(mouse->xPos, mouse->yPos);
                screenColor_ = Tools::screenToColor(mouse->xPos, mouse->yPos);
                entityId_ = Tools::colorToId(screenColor_);
                if (selectedEntityId_ > -1) {
                    board_.moveTile(selectedEntityId_, screentoWorldPos_);
                    return true;
                }
                break;
            }
            case MouseButton: {
                auto mouse = dynamic_cast<const MouseButtonEvent *>(&event);
                buttonPress = mouse->press_release == 0 && entityId_ != -1;
                if (buttonPress) {
                    selectedEntityId_ = entityId_;
                    return true;
                }
                if (mouse->press_release == 0 && selectedEntityId_ != -1) {
                    selectedEntityId_ = -1;
                    return true;
                }
                break;
            }
            case MouseScrolled: {
                auto mouse = dynamic_cast<const MouseScrollEvent *>(&event);
                break;
            }
            default:
                break;
        }
        return false;
    }

private:
    GameGui gui_{};
    Camera camera_{{4, 4, 12}};
    Tile board_{};
    float deltaFrmame_{0};
    int entityId_{-1};
    int selectedEntityId_{-1};
    bool buttonPress{false};
    glm::vec3 screentoWorldPos_{};
    glm::vec3 screenColor_{};
};


#endif //TFG_WORLD_H
