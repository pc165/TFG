#ifndef TFG_SUDOKU_H
#define TFG_SUDOKU_H

#include "Logger.h"
#include "Layer.h"
#include "Cube.h"
#include <filesystem>

class Sudoku : Layer {
private:
    bool showDemoWindow;
    Cube c;
public:
    Sudoku();

    void onUpdate(double deltaTimeSeconds) override;

    void onGuiRender() override;

    void onEvent(Event &event) override;
};


#endif //TFG_SUDOKU_H
