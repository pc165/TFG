#include "Sudoku.h"

Sudoku::Sudoku() : board() {
    int number = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board.addTile({j, i, 0}, number % 10);
            number++;
        }
    }
}

void Sudoku::drawGUI() {
    GameGui::begin();
    GameGui::showOverlay(nullptr, [this]() {
        ImGui::Text("S2W: (%s)", glm::to_string(this->screentoWorldPos).c_str());
        ImGui::Text("S2C: (%s)", glm::to_string(this->screenColor).c_str());
        ImGui::Text("Object id: (%d)", objectId);
        ImGui::Text("Tile value: (%d)", board.getTileValue(objectId));
    });
    GameGui::end();
}

void Sudoku::draw() {
    board.draw();
}

void Sudoku::drawPickObject() {
    board.drawPickObject();
}

bool Sudoku::onEvent(const Event &event, double deltatime) {
    switch (event.type) {
        case Key: {
            auto key = dynamic_cast<const KeyEvent *>(&event);
            if (key->key == GLFW_KEY_2 && key->press_release_repeat == 1) {
                Tools::setFreeCamera(false);
            }
            if (key->key == GLFW_KEY_1 && key->press_release_repeat == 1) {
                Tools::setFreeCamera(true);
            }
            break;
        }
        case MouseMoved: {
            auto mouse = dynamic_cast<const MouseMoveEvent *>(&event);
            screentoWorldPos = Tools::screenToWorld(mouse->xPos, mouse->yPos);
            screenColor = Tools::screenToColor(mouse->xPos, mouse->yPos);
            objectId = Tools::colorToId(screenColor);
            break;
        }
        case MouseButton: {
            auto mouse = dynamic_cast<const MouseButtonEvent *>(&event);
            break;
        }
        default:
            break;
    }
    return false;
}
