#ifndef TFG_SUDOKU_H
#define TFG_SUDOKU_H

#include "GameGui.h"
#include "Cube.h"

class Sudoku {
public:
    explicit Sudoku(GLFWwindow *window) : window(nullptr), events(nullptr), gui(window), demo(true) {
        gui.configure();
        events = (EventStruct *) glfwGetWindowUserPointer(window);
    };

    void render(double deltaSeconds) {
        c.draw();
        gui.begin();
        ImGui::ShowDemoWindow(&demo);
        gui.end();
    }

private:
    Cube c;
    GLFWwindow *window;
    EventStruct *events;
    GameGui gui;
    bool demo;
};


#endif //TFG_SUDOKU_H
