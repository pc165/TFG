#ifndef TFG_GAMEGUI_H
#define TFG_GAMEGUI_H

#include "OpenGL.h"
#include "Logger.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

class GameGui {
private:
    static GLFWwindow *window_;
public:
    GameGui() {
    };

    ~GameGui();

    static void begin();

    static void end();

    static void configure(GLFWwindow *window);

    static void showOverlay(bool *p_open);
};

#endif //TFG_GAMEGUI_H
