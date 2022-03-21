#include "Sudoku.h"

Sudoku::Sudoku() : Layer("Sudoku"), showDemoWindow(true), defaultShader(), vb(), ib() {
    GLfloat triangle[] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f,
    };

    GLuint index[] = {
            0, 1, 2,
            2, 3, 0
    };
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    vb.create(triangle, sizeof(triangle), GL_STATIC_DRAW);
    ib.create(index, 6, GL_STATIC_DRAW);
    ib.bind();
    vb.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
}


void Sudoku::onUpdate(double deltaTimeSeconds) {
    defaultShader.bind();
    glBindVertexArray(VertexArrayID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Sudoku::onAttach() {
}

void Sudoku::onDetach() {
}

void Sudoku::onGuiRender() {
    Layer::onGuiRender();
    ImGui::ShowDemoWindow(&showDemoWindow);
}

void Sudoku::onEvent(Event &event) {
}

