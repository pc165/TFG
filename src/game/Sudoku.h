#ifndef TFG_SUDOKU_H
#define TFG_SUDOKU_H

#include "GameGui.h"
#include "Cube.h"

class Sudoku {
public:
    explicit Sudoku(GLFWwindow *window) : camera(45, 4, 3), c(&camera), window(nullptr), events(nullptr),
                                          gui(window),
                                          demo(true), overlay(true) {
        GameGui::configure();
        events = (EventStruct *) glfwGetWindowUserPointer(window);
        events->camera = &camera;

        glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int x, int y) {
            auto s = (EventStruct *) glfwGetWindowUserPointer(window);
            s->camera->setPerspectiveProjection(45, (float) x, (float) y);
        });
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

private:
    glm::vec3 cameraPosition{0, 10, 0.01};
    glm::vec3 cameraCenter{0, 0, 0};
    glm::vec<3, int> cameraUp{0, 1, 0};
    Camera camera;
    Cube c;
    GLFWwindow *window;
    EventStruct *events;
    GameGui gui;
    bool demo, overlay;
};


#endif //TFG_SUDOKU_H
