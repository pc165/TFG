#ifndef TFG_SUDOKU_H
#define TFG_SUDOKU_H

#include <ctype.h>
#include "../engine/IncludeLibraries.h"
#include "../engine/Layer.h"
#include "../engine/Shader.h"
#include "../engine/Buffer.h"

class Sudoku : Layer {
public:
    Sudoku();

    ~Sudoku() = default;

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
