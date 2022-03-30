#include "OpenGL.h"
#include "Logger.h"
#include "Utils.h"
#include "Sudoku.h"
#include "Window.h"

int main(int, char *[]) {
    InitLogger();
    auto window = InitWindow("TFG");
    ConfigureEvents(window);
    GameGui::configure(window);
    auto &windowProps = *reinterpret_cast<WindowStruct *> (glfwGetWindowUserPointer(window));
    auto &eventsQueue = windowProps.eventCallbackQueue;
    double lastFrameTime = 0;
    {
        Sudoku game;
        glClearColor(0, 0, 0.5, 0);
        do {
            double timeSeconds = glfwGetTime();
            double deltaTime = timeSeconds - lastFrameTime;
            lastFrameTime = timeSeconds;
            game.render(deltaTime);
            while (!eventsQueue.empty()) {
                auto event = std::move(eventsQueue.front());
                if (event->type == EventType::WindowClose)
                    windowProps.shouldClose = true;
                game.onEvent(*event);
                eventsQueue.pop();
            }
            glfwSwapBuffers(window);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glfwPollEvents();
        } while (!windowProps.shouldClose);
    }

    DestroyWindow(window);
    return 0;
}
