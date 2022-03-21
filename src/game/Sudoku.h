#ifndef TFG_SUDOKU_H
#define TFG_SUDOKU_H

#include "../engine/Layer.h"

class Sudoku : Layer {
public:
    Sudoku();

    ~Sudoku() override;

    void onAttach() override;

    void onDetach() override;

    void onUpdate(double deltaTimeSeconds) override;

    void onGuiRender() override;

    void onEvent(Event &event) override;

private:
    bool showDemoWindow;
};


#endif //TFG_SUDOKU_H
