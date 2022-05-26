#ifndef TFG_WORLD_H
#define TFG_WORLD_H

#include "render/GameGui.h"
#include "render/Board.h"
#include "Sudoku.h"
#include "render/Crosshair.h"
#include "Utils.h"


class World {
public:
    explicit World() {
        Injector::camera = &this->camera_;

        assert(Injector::camera != nullptr);
        assert(Injector::eventState != nullptr);
        assert(Injector::window != nullptr);
        assert(Injector::windowStruct != nullptr);
//        tfg::setFreeCamera(true);

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

        eventState_->setCallback([this](float frameTime) {
            onUpdate(frameTime);
            camera_.onUpdate(frameTime);
        });

        sudoku_.setupSudoku(s, board_);
    }

    void gameLoop() {
        float t0 = 0;
        while (!Injector::windowStruct->shouldClose) {
            // Picking object
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            tfg::setClearColor({1.0f, 1.0f, 1.0f, 1.0f});
            board_.drawBoard(true);

            // update state
            eventState_->onUpdate(frametime_);
            // normal rendering
            glfwPollEvents();
            auto t1 = glfwGetTime();
            glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            tfg::setClearColor({0.5f, 0.5f, 0.5f, 0.0f});

            // draw boar and crosshair
            board_.drawBoard();
            crosshairRender_.draw();
            frametime_ = t1 - t0;
            t0 = t1;
            // draw gui
            GameGui::drawwGUI([this]() { guiOverlay(); }, [this]() { guiWindow(); });

            glfwSwapBuffers(Injector::window);
        };
    };

    void guiOverlay() const {
        ImGui::Text("World: (%0.2f,%0.2f,%0.2f)", screentoWorldPos_.x, screentoWorldPos_.y, screentoWorldPos_.z);
        ImGui::Text("Color: (%0.2f,%0.2f,%0.2f)", screenColor_.x, screenColor_.y, screenColor_.z);
        ImGui::Text("Hovered entity: (%d)", hoveredEntity);
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
        auto data = board_.getTile(hoveredEntity);
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

    bool onUpdate(float deltatme) {
        if (eventState_->keyPressed(GLFW_KEY_ESCAPE)) {
            Injector::windowStruct->shouldClose = true;
        }

        if (eventState_->keyPressed(GLFW_KEY_F1)) {
            tfg::setFreeCamera(!Injector::windowStruct->isFreeCamera);
        }

        if (eventState_->mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && selectedEntityId_ == -1 && hoveredEntity != -1) {
            selectedEntityId_ = hoveredEntity;
        } else if (!eventState_->mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
            selectedEntityId_ = -1;
        }

        if (eventState_->isMouseMoved()) {
            auto mouse = eventState_->getMousePosition();
            screentoWorldPos_ = tfg::screenToWorld(mouse.x, mouse.y);
            screenColor_ = tfg::screenToColor(mouse.x, mouse.y);
            hoveredEntity = tfg::colorToId(screenColor_);

            auto nearesTile = board_.nearestTile(screentoWorldPos_, selectedEntityId_);
            if (nearesTile != nearesTile_) {
                if (nearesTile_)
                    nearesTile_->isSelected = false;
                nearesTile_ = nearesTile;
            }

            if (nearesTile_) {
                nearesTile_->isSelected = true;
            }

            if (selectedEntityId_ != -1) {
                board_.moveTile(selectedEntityId_, screentoWorldPos_);
            }
        }
        return false;
    }

private:
    GameGui gui_{};
    Camera camera_{Injector::eventState, {4 * 4, -4 * 4, 35}};
    EventState *eventState_{Injector::eventState};
    Board board_{};
    Crosshair crosshairRender_{};
    Sudoku sudoku_{};
    float frametime_{0};
    int hoveredEntity{-1};
    int selectedEntityId_{-1};
    Tile *nearesTile_{nullptr};
    glm::vec3 screentoWorldPos_{};
    glm::vec3 screenColor_{};
};


#endif //TFG_WORLD_H
