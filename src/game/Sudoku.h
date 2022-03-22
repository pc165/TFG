#ifndef TFG_SUDOKU_H
#define TFG_SUDOKU_H

#include "IncludeLibraries.h"
#include "Layer.h"
#include "Shader.h"
#include "Buffer.h"

class Sudoku : Layer {
public:
    Sudoku();

    ~Sudoku() override = default;

    void onAttach() override;

    void onDetach() override;

    void onUpdate(double deltaTimeSeconds) override;

    void onGuiRender() override;

    void onEvent(Event &event) override;

private:
    bool showDemoWindow;
    Shader defaultShader;
    VertexBuffer vb;
    IndexBuffer<uint32_t> ib;
    GLuint VertexArrayID;
};


#endif //TFG_SUDOKU_H
