#ifndef TFG_WORLD_H
#define TFG_WORLD_H

#include "GameGui.h"
#include "Board.h"
#include "Event.h"
#include "Tools.h"
#include "Sudoku.h"
#include <cmath>
#include <glm/gtx/string_cast.hpp>


class World {
public:
    explicit World() {
        Tools::camera = &this->camera_;
        assert(Tools::camera != nullptr);
        assert(Tools::window != nullptr);
        assert(Tools::windowStruct != nullptr);
        std::vector<std::vector<int>> s =
                {{9, 0, 4, 5, 7, 6, 2, 1, 3},
                 {5, 1, 3, 4, 8, 2, 9, 6, 7},
                 {7, 2, 6, 1, 3, 9, 5, 0, 8},
                 {6, 3, 1, 9, 4, 7, 8, 5, 2},
                 {4, 9, 5, 2, 6, 8, 3, 7, 1},
                 {8, 7, 2, 0, 5, 1, 6, 9, 4},
                 {2, 5, 7, 6, 1, 3, 4, 8, 9},
                 {3, 6, 8, 7, 9, 4, 1, 2, 5},
                 {1, 4, 9, 8, 2, 5, 7, 3, 6},
                };
        sudoku_.setupSudoku(s, board_);
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
            GameGui::drawwGUI([this]() { guiOverlay(); }, [this]() { guiWindow(); });
            t0 = t1;
            glfwSwapBuffers(Tools::window);
        };
    };

    void guiOverlay() {
        ImGui::Text("S2W: (%0.1f,%0.1f,%0.1f)", screentoWorldPos_.x, screentoWorldPos_.y, screentoWorldPos_.z);
        ImGui::Text("S2C: (%0.1f,%0.1f,%0.1f)", screenColor_.x, screenColor_.y, screenColor_.z);
        ImGui::Text("Mouse button: (%d)", buttonPress);
        ImGui::Text("Hovered entity: (%d)", entityId_);
        ImGui::Text("Selected entity: (%d)", selectedEntityId_);
    }

    void guiWindow() {
        ImGui::Begin("Controls");
        ImGui::InputFloat("FoV", &camera_.fov);
        ImGui::SliderFloat("Yaw", &camera_.yaw, 0, 360);
        ImGui::SliderFloat("Pitch", &camera_.pitch, 0, 360);
        ImGui::SliderFloat3("Position", glm::value_ptr(camera_.pos), -10, 10);
        ImGui::SliderFloat3("Center", glm::value_ptr(camera_.center), -10, 10);
        ImGui::SliderFloat3("Up", glm::value_ptr(camera_.up), -1, 1);
        camera_.updateCameraVectors();
        ImGui::Separator();
        auto data = board_.getTile(entityId_);
        if (data != nullptr) {
            ImGui::Text("Position (%0.1f,%0.1f,%0.1f)", data->cube.position.x, data->cube.position.y, data->cube.position.z);
            ImGui::Text("Value %d (%d,%d)", data->numericalValue, data->row, data->col);
        }
        ImGui::End();
    }

    bool onEvent(const Event &event) {
        switch (event.type) {
            case Key: {
                auto key = dynamic_cast<const KeyEvent *>(&event);
                if (key->key == GLFW_KEY_ESCAPE && key->press_release_repeat == 0) {
                    Tools::windowStruct->shouldClose = true;
                }

                if (key->key == GLFW_KEY_1 && key->press_release_repeat == 0) {
                    Tools::setFreeCamera(!Tools::windowStruct->isFreeCamera);
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
                    return false;
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
                if (mouse->press_release == 1 && selectedEntityId_ != -1) {
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
    Camera camera_{{4 * 3, -4 * 3, 35}};

    Board board_{};
    Sudoku sudoku_{};

    float deltaFrmame_{0};

    int entityId_{-1};
    int selectedEntityId_{-1};
    bool buttonPress{false};
    glm::vec3 screentoWorldPos_{};
    glm::vec3 screenColor_{};
};


#endif //TFG_WORLD_H
