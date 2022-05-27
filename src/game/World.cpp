#include "World.h"

World::World() {
    Injector::camera = &this->camera_;

    assert(Injector::camera != nullptr);
    assert(Injector::eventState != nullptr);
    assert(Injector::window != nullptr);
//        tfg::setFreeCamera(true);

    sudokuValues_ = {
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
    sudoku_.setupSudoku(sudokuValues_, board_);

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

        // draw board
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
    ImGui::Text("Hovered entity: (%d)", hoveredTile_ ? hoveredTile_->entityId : -1);
    ImGui::Text("Selected entity: (%d)", selectedTile_ ? selectedTile_->entityId : -1);
}

void World::guiWindow() {
    if (sudoku_.isDone()) {
        ImGui::OpenPopup("Congurations");
    }

    if (ImGui::BeginPopupModal("Congurations", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        tfg::setFreeCamera(false);
        ImGui::Text("Sudoku completed!");
        if (ImGui::Button("Restart")) {
            sudoku_.restartSudoku(sudokuValues_);
            ImGui::CloseCurrentPopup();
            tfg::setFreeCamera(true);
        }

        if (ImGui::Button("New Sudoku")) {
            Sudoku::randomSudokuGenerator(sudokuValues_);
            ImGui::CloseCurrentPopup();
            tfg::setFreeCamera(true);
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
    if (hoveredTile_) {
        auto row = hoveredTile_->row, col = hoveredTile_->col;
        ImGui::Separator();
        ImGui::Text("Position (%0.2f,%0.2f,%0.2f)", hoveredTile_->cube.position.x, hoveredTile_->cube.position.y, hoveredTile_->cube.position.z);
        ImGui::Text("Cell (%d,%d)", row, col);
        ImGui::Text("Value %d (%d)", hoveredTile_->numericalValue, !hoveredTile_->isDeck ? sudoku_.getSolution(row, col) : 0);
        ImGui::Text("Hints %d ", hoveredTile_->hints);
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

    // update status
    if (eventState_->isMouseMoved() && eventState_->isMouseInWindow()) {
        auto mouse = eventState_->getMousePosition();
        screentoWorldPos_ = tfg::screenToWorld(mouse.x, mouse.y);
        screenColor_ = tfg::screenToColor(mouse.x, mouse.y);

        // TODO Hide decimal number, show braille
        if (hoveredTile_) {
            hoveredTile_->isSelected = false;
        }

        hoveredTile_ = board_.getTile(tfg::colorToId(screenColor_));

        // TODO Hide braille, show decimal
        if (hoveredTile_) {
            hoveredTile_->isSelected = true;
        }

        if (selectedTile_) {

            // skip the selected tile and the deck
            auto nearesTile = board_.nearestTile(screentoWorldPos_, [this](tfg::Tile const &tile) {
                return tile.entityId != selectedTile_->entityId && !tile.isDeck;
            });

            // update nearest tile
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
            selectedTile_->isSelected = false;
            selectedTile_->updatePosition(screentoWorldPos_);
        }
    }

    if (eventState_->mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
        // select a deck tile
        if (!selectedTile_ && hoveredTile_ && hoveredTile_->isDeck) {
            selectedTile_ = hoveredTile_;
        }
    }

    // show hints on left click
    if (eventState_->mouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && hoveredTile_ && !hoveredTile_->isDeck) {
        int row = hoveredTile_->row, col = hoveredTile_->col;
        auto solution = sudoku_.getSolution(row, col);

        // if there's a solution update hints
        if (solution) {
            auto &tile = hoveredTile_;
            tile->numericalValue = solution;
            tile->hints++;
            tile->hints %= NUMBER_TO_BRAILLE[solution].size();
            if (tile->hints == 0) tile->numericalValue = 0;
        }
    }

    // drop the selected tile
    if (!eventState_->mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && selectedTile_) {

        // set the sudoku value
        if (nearesTile_ && !nearesTile_->isDeck) {
            assert(selectedTile_->isDeck);
            bool result = sudoku_.setNumber(nearesTile_->row, nearesTile_->col, selectedTile_->numericalValue);
            selectedTile_->hints = 0;
            LOG_INFO("Set number {}", result);
        }

        // reset position
        selectedTile_->moveBack(board_.offset_);

        // unselect tiles
        if (nearesTile_) nearesTile_->isSelected = false;
        selectedTile_ = nullptr;
        hoveredTile_ = nullptr;
    }
    return false;
}
