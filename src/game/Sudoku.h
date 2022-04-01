#ifndef TFG_SUDOKU_H
#define TFG_SUDOKU_H

#include "GameGui.h"
#include "Cube.h"
#include "Event.h"

class Sudoku {
public:
    Sudoku() : camera(45, 4, 3), c(&camera), demo(true), overlay(true) {
    };

    void render(double deltaSeconds) {
        c.draw();
        GameGui::begin();
        ImGui::SliderFloat3("Position", (float *) &cameraPosition, -10, 10);
        ImGui::SliderFloat3("Center", (float *) &cameraCenter, -10, 10);
        ImGui::SliderInt3("Up", (int *) &cameraUp, -1, 1);
        ImGui::ShowDemoWindow(&demo);
        GameGui::showOverlay(&overlay);
        GameGui::end();
        camera.SetPosition(cameraPosition);
        camera.setCenter(cameraCenter);
        camera.setUp(cameraUp);
    }

    void onEvent(Event &event) {
        switch (event.type) {
            case WindowClose:
                break;
            case WindowResize: {
                auto winRes = dynamic_cast<WindowResizeEvent *>(&event);
                camera.setPerspectiveProjection(45, (float) winRes->width, (float) winRes->height);
                break;
            }
            case Key:
                break;
            case MouseMoved:
                break;
            case MouseScrolled:
                break;
            case MouseButton:
                break;
        }
    }

private:
    glm::vec3 cameraPosition{0, 10, 0.01};
    glm::vec3 cameraCenter{0, 0, 0};
    glm::vec<3, int> cameraUp{0, 1, 0};
    Camera camera;
    Cube c;
    bool demo, overlay;
};


#endif //TFG_SUDOKU_H
