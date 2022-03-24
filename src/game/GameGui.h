#ifndef TFG_GAMEGUI_H
#define TFG_GAMEGUI_H

#include "OpenGL.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

class GameGui {
private:
    GLFWwindow *window_;
    int display_w{}, display_h{};
public:
    explicit GameGui(GLFWwindow *window);

    ~GameGui();

    void begin();

    void end();

    void configure();
};

#endif //TFG_GAMEGUI_H
