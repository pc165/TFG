#ifndef TFG_GUI_H
#define TFG_GUI_H

#include "IncludeLibraries.h"
#include "OpenGL.h"
#include "Layer.h"

class Gui : Layer {
private:
    GLFWwindow *window_;
    int display_w, display_h;
public:
    Gui();

    ~Gui() override;

    void begin();

    void end();

    void onAttach() override;

    void onDetach() override;
};

#endif //TFG_GUI_H
