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
    auto &eventsQueue = windowProps.eventCallbackQueue;
    {
        Sudoku game;
        double t0 = 0;
        while (!windowProps.shouldClose) {
            glfwPollEvents();
            auto t1 = glfwGetTime();
            auto delta = t1 - t0;
            game.render(delta);
            t0 = t1;
            while (!eventsQueue.empty()) {
                auto event = std::move(eventsQueue.front());
                game.onEvent(*event, delta);
                eventsQueue.pop();
            }
            glfwSwapBuffers(window);
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        };
    }

    DestroyWindow(window);
    return 0;
}
