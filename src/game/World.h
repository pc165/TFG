#ifndef TFG_WORLD_H
#define TFG_WORLD_H

#include "GameGui.h"
#include "Board.h"
#include "Event.h"
#include "Sudoku.h"
#include "Crosshair.h"
#include "Utils.h"


class World {
public:
    explicit World() {
        Injector::camera = &this->camera_;
        assert(Injector::camera != nullptr);
        assert(Injector::window != nullptr);
        assert(Injector::windowStruct != nullptr);
        std::vector<std::vector<int>> s = {
                {9, 0, 4, 5, 7, 6, 2, 1, 3},
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
        tfg::setFreeCamera(true);
    }

    void gameLoop() {
        auto windowProps = Injector::windowStruct;
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
            crosshair.draw();
            GameGui::drawwGUI([this]() { guiOverlay(); }, [this]() { guiWindow(); });
            t0 = t1;
            glfwSwapBuffers(Injector::window);
        };
    };

    void guiOverlay() const {
        ImGui::Text("World: (%0.2f,%0.2f,%0.2f)", screentoWorldPos_.x, screentoWorldPos_.y, screentoWorldPos_.z);
        ImGui::Text("Color: (%0.2f,%0.2f,%0.2f)", screenColor_.x, screenColor_.y, screenColor_.z);
        ImGui::Text("Mouse button: (%d)", buttonPress);
        ImGui::Text("Hovered entity: (%d)", entityId_);
        ImGui::Text("Selected entity: (%d)", selectedEntityId_);
    }

    void guiWindow() {
        ImGui::Begin("Controls");
        ImGui::SliderFloat("zNear", &camera_.zNear_, 0.001f, 200);
        ImGui::SliderFloat("zFar", &camera_.zFar_, 0.001f, 200);
        ImGui::InputFloat("FoV", &camera_.fov_);
        ImGui::SliderFloat("Yaw", &camera_.yaw_, 0, 360);
        ImGui::SliderFloat("Pitch", &camera_.pitch_, -89, 89);
        ImGui::SliderFloat3("Position", &camera_.position_.x, -10, 10);
        ImGui::SliderFloat3("Center", &camera_.center_.x, -10, 10);
        ImGui::SliderFloat3("Up", &camera_.up_.x, -1, 1);
        camera_.updateCameraVectors();
        auto data = board_.getTile(entityId_);
        if (data != nullptr) {
            ImGui::Separator();
            ImGui::Text("Position (%0.2f,%0.2f,%0.2f)", data->cube.position.x, data->cube.position.y, data->cube.position.z);
            ImGui::Text("Value %d (%d,%d)", data->numericalValue, data->row, data->col);
        }
        if (nearesTile_) {
            ImGui::Separator();
            ImGui::Text("Neares entity %d", nearesTile_->entityId);
        }
        ImGui::End();
    }

    bool onEvent(const Event &event) {
        switch (event.type) {
            case Key: {
                auto key = dynamic_cast<const KeyEvent *>(&event);
                if (key->key == GLFW_KEY_ESCAPE && key->press_release_repeat == 0) {
                    Injector::windowStruct->shouldClose = true;
                }

                if (key->key == GLFW_KEY_F1 && key->press_release_repeat == 0) {
                    tfg::setFreeCamera(!Injector::windowStruct->isFreeCamera);
                    return true;
                }
                break;
            }
            case MouseMoved: {
                auto mouse = dynamic_cast<const MouseMoveEvent *>(&event);
                screenColor_ = tfg::screenToColor(mouse->xPos, mouse->yPos);
                entityId_ = tfg::colorToId(screenColor_);

                glm::vec3 point;
                Tile *tile;
                if ((tile = board_.getTile(entityId_)) != nullptr)
                    point = tile->cube.position;
                else
                    point = {camera_.position_.x, camera_.position_.y, camera_.zFar_};

                screentoWorldPos_ = tfg::screenToWorld(mouse->xPos, mouse->yPos, point);

                auto nearesTile = board_.nearestTile(screentoWorldPos_, selectedEntityId_);
                if (nearesTile != nearesTile_) {
                    if (nearesTile_)
                        nearesTile_->isSelected = false;
                    nearesTile_ = nearesTile;
                }

                if (nearesTile_) {
                    nearesTile_->isSelected = true;
                }

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
    Camera camera_{{4 * 4, -4 * 4, 35}};

    Board board_{};
    Sudoku sudoku_{};
    Crosshair crosshair{};

    float deltaFrmame_{0};

    int entityId_{-1};
    int selectedEntityId_{-1};
    bool buttonPress{false};
    Tile *nearesTile_{nullptr};

    glm::vec3 screentoWorldPos_{};
    glm::vec3 screenColor_{};
};


#endif //TFG_WORLD_H
