#include "OpenGL.h"
#include "Logger.h"
#include "Sudoku.h"
#include "Window.h"

int main(int, char *[]) {
    InitLogger();
    auto window = InitWindow("TFG");
    ConfigureEvents(window);
    GameGui::configure(window);
    auto &windowProps = *reinterpret_cast<WindowStruct *> (glfwGetWindowUserPointer(window));
    {
        Sudoku game;
        double t0 = 0, delta = 0;
        windowProps.eventCallback = [&game, &delta](const Event &event) {
            game.onEvent(event, delta);
        };
        while (!windowProps.shouldClose) {
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            game.drawPickObject();
            glfwPollEvents();
            glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            auto t1 = glfwGetTime();
            delta = t1 - t0;
            game.draw();
            game.drawGUI();
            t0 = t1;
            glfwSwapBuffers(window);
        };
    }

    DestroyWindow(window);
    return 0;
}
