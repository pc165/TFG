#ifndef TFG_GAMEGUI_H
#define TFG_GAMEGUI_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Utils.h"
#include "Logger.h"

class GameGui {
public:
    GameGui();

    ~GameGui();

    static void drawwGUI(const std::function<void()> &overlay = {}, const std::function<void()> &window = {});
};

#endif //TFG_GAMEGUI_H
