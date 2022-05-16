#include "OpenGL.h"
#include "Logger.h"
#include "Sudoku.h"
#include "Window.h"

int main(int, char *[]) {
    InitLogger();
    InitWindow("TFG");
    ConfigureEvents(Tools::window);
    {
        Camera camera({4, 4, 12});
        Tools::camera = &camera;

        assert(Tools::camera != nullptr);
        assert(Tools::window != nullptr);
        assert(Tools::windowStruct != nullptr);

        GameGui::configure(Tools::window);
        Sudoku game;

        auto windowProps = Tools::windowStruct;
        double t0 = 0, delta = 0;

        windowProps->eventCallback = [&game, &delta, &camera](const Event &event) {
            camera.onEvent(event, delta);
            game.onEvent(event, delta);
        };

        while (!windowProps->shouldClose) {
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
            glfwSwapBuffers(Tools::window);
        };
    }
    DestroyWindow(Tools::window);
    return 0;
}
