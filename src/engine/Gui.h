#ifndef TFG_GUI_H
#define TFG_GUI_H

#include <GLFW/glfw3.h>
#include "../lib/imgui/imgui.h"
#include "../lib/imgui/backends/imgui_impl_glfw.h"
#include "../lib/imgui/backends/imgui_impl_opengl3.h"

class Gui {
private:
    GLFWwindow *window_;
public:
    explicit Gui(GLFWwindow *window);
    ~Gui();
    void newFrame() const;

    void render() const;
};

#endif //TFG_GUI_H
