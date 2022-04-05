#ifndef TFG_SUDOKU_H
#define TFG_SUDOKU_H

#include "GameGui.h"
#include "Cube.h"
#include "Event.h"
#include "EventState.h"


class Sudoku {
public:
    explicit Sudoku(Camera &camera) : camera(camera), c(&camera) {
        c.add();
        c.add({1, 2, 3});
    };

    void render(double deltaSeconds) {
        c.draw();
        GameGui::begin();
        ImGui::Begin("Camera");
        ImGui::SliderFloat3("Position", (float *) &camera.pos, -10, 10);
        ImGui::SliderFloat3("Center", (float *) &camera.front, -10, 10);
        ImGui::SliderFloat3("Up", (float *) &camera.up, -1, 1);
        ImGui::End();

        ImGui::Begin("Model");
        ImGui::SliderInt("Cube id", &selector, 0, c.size - 1);
        ImGui::SliderFloat3("Position", (float *) &c.position_[selector], -10, 10);
        ImGui::SliderFloat3("Scale", (float *) &c.scale_[selector], 1, 10);
        ImGui::SliderFloat3("Axis", (float *) &c.rotationAxis_[selector], -1, 1);
        ImGui::SliderFloat("Degrees", &c.degrees_[selector], -180, 180);
        ImGui::End();

        GameGui::showOverlay(nullptr);
        GameGui::end();
    }


    bool onEvent(const Event &event, double deltatime) {
        switch (event.type) {
            case Key: {
                auto key = dynamic_cast<const KeyEvent *>(&event);
                break;
            }
            case MouseMoved: {
                auto mouse = dynamic_cast<const MouseMoveEvent *>(&event);
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
    int selector = 0;
    Camera &camera;
    Cube c;
    GameGui gui{};
};


#endif //TFG_SUDOKU_H
