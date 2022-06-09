#include "World.h"
#include "Audio.h"

World::World() {
    GlobalOptions.camera = &this->camera_;
    assert(GlobalOptions.camera != nullptr);
    assert(Injector::eventState != nullptr);
    assert(Injector::window != nullptr);
    tfg::setFreeCamera(GlobalOptions.isFreeCamera);

    sudokuValues_ = {
            {9, 0, 4, 5, 7, 6, 2, 1, 3}, // 8
            {5, 1, 3, 4, 8, 2, 9, 6, 7},
            {7, 2, 6, 1, 3, 9, 5, 4, 8},
            {6, 3, 1, 9, 4, 7, 8, 0, 2},
            {4, 9, 5, 2, 6, 8, 3, 7, 1},
            {8, 7, 2, 3, 5, 1, 6, 9, 4},
            {2, 5, 0, 6, 1, 3, 4, 8, 9},
            {3, 6, 8, 7, 9, 4, 1, 2, 5},
            {1, 4, 9, 8, 2, 5, 7, 3, 6},
    };
    sudoku_.setupSudoku(sudokuValues_, board_);
    sudokuPath_.resize(100);
    eventState_->setCallback([this](float frameTime) {
        onUpdate(frameTime);
        camera_.onUpdate(frameTime);
    });

    GlobalOptions.light.position = camera_.position_;
    GlobalOptions.light.direction = camera_.center_;
    GlobalOptions.light.cutOff = glm::cos(glm::radians(cutoffDegress));
    GlobalOptions.light.outerCutOff = glm::cos(glm::radians(outcutoffDegress));
    GlobalOptions.light.ambient = {0.5f, 0.5f, 0.5f};
    GlobalOptions.light.diffuse = {0.8f, 0.8f, 0.8f};
    GlobalOptions.light.specular = {1.0f, 1.0f, 1.0f};
    GlobalOptions.light.constant = 1.0f;
    GlobalOptions.light.linear = 0.01f;
    GlobalOptions.light.quadratic = 0.0f;
}

void World::gameLoop() {
    float t0 = 0;
    while (!GlobalOptions.shouldClose) {

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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        tfg::setClearColor({0.1f, 0.1f, 0.1f, 1.0f});

        // draw board
        board_.drawBoard(GlobalOptions.drawPickingObject);
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
        if (ImGui::Button("New Sudoku")) {
            sudoku_.randomSudokuGenerator();
            ImGui::CloseCurrentPopup();
            tfg::setFreeCamera(true);
        }
        ImGui::EndPopup();
    }

    ImGui::Begin("Game menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    {
        if (ImGui::Button("Load sudoku")) {
            sudoku_.loadSudoku(sudokuPath_);
        }
        ImGui::SameLine();
        ImGui::InputText("Path", sudokuPath_.data(), sudokuPath_.capacity());
        if (ImGui::Button("New Sudoku")) {
            sudoku_.randomSudokuGenerator(difficulty);
        }
        ImGui::SameLine();
        ImGui::SliderInt("Difficulty", &difficulty, 0, 2);


        if (ImGui::Checkbox("Lock cursor (F1)", &GlobalOptions.isFreeCamera)) {
            tfg::setFreeCamera(GlobalOptions.isFreeCamera);
            if (GlobalOptions.isFreeCamera)
                ImGui::SetWindowFocus();
        }
        if (ImGui::Button("Reset camera position")) {
            camera_.position_ = {4 * board_.offset_, -4 * board_.offset_, 35};
            camera_.pitch_ = 0.0f;
            camera_.yaw_ = 270.0f;
        }
        ImGui::Checkbox("Show game status", &GlobalOptions.gameStatus);
        ImGui::Checkbox("Draw Picking object", &GlobalOptions.drawPickingObject);
        ImGui::Checkbox("Show Controls", &GlobalOptions.controls);
    }
    ImGui::End();

    if (GlobalOptions.gameStatus) {
        ImGui::Begin("Status", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Neares entity %d", nearesTile_ ? nearesTile_->entityId : -1);
        if (hoveredTile_) {
            auto row = hoveredTile_->row, col = hoveredTile_->col;
            ImGui::Text("Position (%0.2f,%0.2f,%0.2f)", hoveredTile_->cube.position.x, hoveredTile_->cube.position.y,
                        hoveredTile_->cube.position.z);
            ImGui::Text("Cell (%d,%d)", row, col);
            ImGui::Text("Value %d (%d)", hoveredTile_->numericalValue, !hoveredTile_->isDeck ? sudoku_.getSolution(row, col) : 0);
            ImGui::Text("Hints %d ", hoveredTile_->hints);
        } else {
            ImGui::Text("Position (null)");
            ImGui::Text("Cell (-1,-1)");
            ImGui::Text("Value -1 (-1)");
            ImGui::Text("Hints -1");
        }
        ImGui::End();
    }

    if (GlobalOptions.controls) {
        ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Camera");
        ImGui::SliderFloat("Yaw", &camera_.yaw_, 0, 360);
        ImGui::SliderFloat("Pitch", &camera_.pitch_, -89, 89);
        ImGui::SliderFloat3("Position", &camera_.position_.x, -10, 10);
        ImGui::SliderFloat3("Center", &camera_.center_.x, -10, 10);
        ImGui::SliderFloat3("Up", &camera_.up_.x, -1, 1);
        ImGui::SliderFloat("zNear", &camera_.zNear_, 0.001f, 200);
        ImGui::SliderFloat("zFar", &camera_.zFar_, 0.001f, 200);
        ImGui::InputFloat("FoV", &camera_.fov_);
        camera_.updateCameraVectors();

        ImGui::Text("Draw Normals");
        ImGui::Checkbox("Cube normals", &GlobalOptions.drawCubeNormals);
        ImGui::Checkbox("Sphere normals", &GlobalOptions.drawSphereNormals);
        ImGui::Checkbox("Plane normals", &GlobalOptions.drawPlaneNormals);

        ImGui::Text("Light");
        ImGui::Checkbox("Lock light to camera", &GlobalOptions.lockLightPosition);
        if (GlobalOptions.lockLightPosition) {
            GlobalOptions.light.position = camera_.position_;
            GlobalOptions.light.direction = camera_.center_;
        }
        ImGui::SliderFloat3("Position", &GlobalOptions.light.position.x, 0, 40);
        ImGui::SliderFloat3("Ambient", &GlobalOptions.light.ambient.x, 0, 1);
        ImGui::SliderFloat3("Direction", &GlobalOptions.light.direction.x, -1, 1);
        ImGui::SliderFloat3("Specular", &GlobalOptions.light.specular.x, 0, 1);
        ImGui::SliderFloat3("Diffuse", &GlobalOptions.light.diffuse.x, 0, 1);
        ImGui::SliderFloat("Cutoff", &cutoffDegress, 0, 90);
        ImGui::SliderFloat("OuterCutoff", &outcutoffDegress, 0, 90);
        GlobalOptions.light.cutOff = glm::cos(glm::radians(cutoffDegress));
        GlobalOptions.light.outerCutOff = glm::cos(glm::radians(outcutoffDegress));
        ImGui::SliderFloat("Constant", &GlobalOptions.light.constant, 0, 0.1);
        ImGui::SliderFloat("Liniear", &GlobalOptions.light.linear, 0, 0.1);
        ImGui::SliderFloat("Quadratic", &GlobalOptions.light.quadratic, 0, 0.1);
        ImGui::End();
    }
}

bool World::onUpdate(float deltatme) {
    // close window
    if (eventState_->keyPressed(GLFW_KEY_ESCAPE)) {
        GlobalOptions.shouldClose = true;
    }

    // toogle camera mode
    if (eventState_->keyPressed(GLFW_KEY_F1)) {
        tfg::setFreeCamera(!GlobalOptions.isFreeCamera);
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

    if (eventState_->mouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && hoveredTile_) {
        auto &tile = hoveredTile_;

        tfg::PlayNumberAudio(tile->numericalValue);

        // show hints on left click
        if (hoveredTile_->isDeck) {
            // if there's a solution update hints
            if (tile->isHintsEnabled) {
                int row = hoveredTile_->row, col = hoveredTile_->col;
                auto solution = sudoku_.getSolution(row, col);
                tile->numericalValue = solution;
                tile->hints++;
                tile->hints %= NUMBER_TO_BRAILLE[solution].size();
                if (tile->hints == 0) tile->numericalValue = 0;
            }
        }
    }

    // drop the selected tile
    if (!eventState_->mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && selectedTile_) {

        // set the sudoku value
        if (nearesTile_ && !nearesTile_->isDeck && !sudoku_.isReadOnly(nearesTile_->row, nearesTile_->col)) {
            assert(selectedTile_->isDeck);
            bool result = sudoku_.setNumber(nearesTile_->row, nearesTile_->col, selectedTile_->numericalValue);
            if (result) {
                nearesTile_->isHintsEnabled = selectedTile_->numericalValue == 0;
                nearesTile_->hints = 0;
            }
            LOG_INFO("Set number {} {}", selectedTile_->numericalValue, result);
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
