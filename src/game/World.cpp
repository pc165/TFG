#include "World.h"

World::World() {
    Injector::camera = &this->camera_;

    assert(Injector::camera != nullptr);
    assert(Injector::eventState != nullptr);
    assert(Injector::window != nullptr);
//        tfg::setFreeCamera(true);

    std::vector<std::vector<int>> s = {
            {9, 0, 4, 5, 7, 6, 2, 1, 3}, // 8
            {5, 1, 3, 4, 8, 2, 9, 6, 7},
            {7, 2, 6, 1, 3, 9, 5, 4, 8},
            {6, 3, 1, 9, 4, 7, 8, 5, 2},
            {4, 9, 5, 2, 6, 8, 3, 7, 1},
            {8, 7, 2, 3, 5, 1, 6, 9, 4},
            {2, 5, 7, 6, 1, 3, 4, 8, 9},
            {3, 6, 8, 7, 9, 4, 1, 2, 5},
            {1, 4, 9, 8, 2, 5, 7, 3, 6},
    };
    sudoku_.setupSudoku(s, board_);

    eventState_->setCallback([this](float frameTime) {
        onUpdate(frameTime);
        camera_.onUpdate(frameTime);
    });

}

void World::gameLoop() {
    float t0 = 0;
    while (!Injector::shouldClose) {

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
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        tfg::setClearColor({0.5f, 0.5f, 0.5f, 1.0f});

        // draw boar and crosshair
        board_.drawBoard();
        frametime_ = t1 - t0;
        t0 = t1;

        // draw gui
        crosshairRender_.draw();
        GameGui::drawwGUI([this]() { guiOverlay(); }, [this]() { guiWindow(); });

        glfwSwapBuffers(Injector::window);
    };
}

void World::guiOverlay() const {
    ImGui::Text("World: (%0.2f,%0.2f,%0.2f)", screentoWorldPos_.x, screentoWorldPos_.y, screentoWorldPos_.z);
    ImGui::Text("Color: (%0.2f,%0.2f,%0.2f)", screenColor_.x, screenColor_.y, screenColor_.z);
    ImGui::Text("Hovered entity: (%d)", hoveredTile_->entityId);
    ImGui::Text("Selected entity: (%d)", selectedTile_->entityId);
}

void World::guiWindow() {
    if (sudoku_.isDone()) {
        ImGui::OpenPopup("Congurations");
    }

    if (ImGui::BeginPopupModal("Congurations", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        auto tmp = Injector::isFreeCamera;
        tfg::setFreeCamera(false);
        ImGui::Text("Sudoku completed!");
        if (ImGui::Button("Restart")) {
            tfg::setFreeCamera(tmp);
            std::vector<std::vector<int>> s = {
                    {9, 0, 4, 5, 7, 6, 2, 1, 3}, // 8
                    {5, 1, 3, 4, 8, 2, 9, 6, 7},
                    {7, 2, 6, 1, 3, 9, 5, 4, 8},
                    {6, 3, 1, 9, 4, 7, 8, 5, 2},
                    {4, 9, 5, 2, 6, 8, 3, 7, 1},
                    {8, 7, 2, 3, 5, 1, 6, 9, 4},
                    {2, 5, 7, 6, 1, 3, 4, 8, 9},
                    {3, 6, 8, 7, 9, 4, 1, 2, 5},
                    {1, 4, 9, 8, 2, 5, 7, 3, 6},
            };
            sudoku_.restartSudoku(s);
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
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
    auto data = hoveredTile_;
    if (data) {
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

bool World::onUpdate(float deltatme) {
    // close window
    if (eventState_->keyPressed(GLFW_KEY_ESCAPE)) {
        Injector::shouldClose = true;
    }

    // toogle camera mode
    if (eventState_->keyPressed(GLFW_KEY_F1)) {
        tfg::setFreeCamera(!Injector::isFreeCamera);
    }

    // reset camera position
    if (eventState_->keyPressed(GLFW_KEY_F2)) {
        camera_.position_ = {4 * board_.offset_, -4 * board_.offset_, 35};
        camera_.pitch_ = 0.0f;
        camera_.yaw_ = 270.0f;
    }

    // select a deck tile
    bool hoveredDeckTile = hoveredTile_ && hoveredTile_->isDeck;
    if (eventState_->mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && !selectedTile_ && hoveredDeckTile) {
        selectedTile_ = hoveredTile_;
    }

    // drop the selected tile
    if (!eventState_->mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && selectedTile_) {

        // set the sudoku value
        if (nearesTile_ && !nearesTile_->isDeck) {
            assert(selectedTile_->isDeck);
            bool result = sudoku_.setNumber(nearesTile_->row, nearesTile_->col, selectedTile_->numericalValue);
            LOG_INFO("Set number {}", result);
        }

        // reset position
        selectedTile_->moveBack(board_.offset_);

        // unselect tiles
        if (nearesTile_) nearesTile_->isSelected = false;
        selectedTile_ = nullptr;
        hoveredTile_ = nullptr;
    }

    // update status
    if (eventState_->isMouseMoved() && eventState_->isMouseInWindow()) {
        auto mouse = eventState_->getMousePosition();
        screentoWorldPos_ = tfg::screenToWorld(mouse.x, mouse.y);
        screenColor_ = tfg::screenToColor(mouse.x, mouse.y);
        hoveredTile_ = board_.getTile(tfg::colorToId(screenColor_));

        // select a tile that is near the selected one
        if (selectedTile_) {
            auto nearesTile = board_.nearestTile(screentoWorldPos_, selectedTile_->entityId);
            if (nearesTile != nearesTile_) {
                if (nearesTile)
                    nearesTile->isSelected = true;
                if (nearesTile_)
                    nearesTile_->isSelected = false;
                nearesTile_ = nearesTile;
            }
        }

        // update position for the selected tile
        if (selectedTile_) {
            selectedTile_->updatePosition(screentoWorldPos_);
            return !Injector::isFreeCamera;
        }
    }
    return false;
}
