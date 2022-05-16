#ifndef TFG_SUDOKU_H
#define TFG_SUDOKU_H

#include "GameGui.h"
#include "Board.h"
#include "Event.h"
#include "Tools.h"
#include <cmath>
#include <glm/gtx/string_cast.hpp>


class Sudoku {
public:
    explicit Sudoku() : camera({0, 0, 5}), board(&camera) {
        board.addTile({0, 0, 0}, 0);
        board.addTile({0, 5, 0}, 0);
    };

    void render() {
        board.draw();
        GameGui::begin();
        ImGui::Begin("Camera");
        ImGui::SliderFloat3("Position", (float *) &camera.pos, -10, 10);
        ImGui::SliderFloat3("Center", (float *) &camera.center, -10, 10);
        ImGui::SliderFloat3("Up", (float *) &camera.up, -1, 1);
        ImGui::End();

        GameGui::showOverlay(nullptr, [this]() {
            ImGui::Text("S2W: (%s)", glm::to_string(this->screentoWorldPos).c_str());
            ImGui::Text("S2C: (%s)", glm::to_string(this->screenColor).c_str());
            ImGui::Text("Object id: (%d)", objectInt);
        });
        GameGui::end();
    }

    void renderPickObject() {
        board.drawPickObject();
    }

    bool onEvent(const Event &event, double deltatime) {
        camera.onEvent(event, deltatime);
        switch (event.type) {
            case Key: {
                auto key = dynamic_cast<const KeyEvent *>(&event);
                break;
            }
            case MouseMoved: {
                auto mouse = dynamic_cast<const MouseMoveEvent *>(&event);
                screentoWorldPos = Tools::screenToWorld(mouse->xPos, mouse->yPos, &camera);
                screenColor = Tools::screenToColor(mouse->xPos, mouse->yPos);
                objectInt = Object::colorToId(screenColor);
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

private:
    glm::vec3 screentoWorldPos{};
    glm::vec3 screenColor{};
    int objectInt = -1;
    Camera camera;
    Board board;
    GameGui gui{};
};


#endif //TFG_SUDOKU_H
