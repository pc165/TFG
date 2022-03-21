#ifndef TFG_GUI_H
#define TFG_GUI_H

#include "IncludeLibraries.h"
#include "Layer.h"

class Gui : Layer {
private:
    GLFWwindow *window_;
public:
    Gui();

    ~Gui() override;

    void begin() const;

    void end() const;

    void onAttach() override;

    void onDetach() override;
};

#endif //TFG_GUI_H
