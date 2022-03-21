#include "Sudoku.h"
#include "../engine/IncludeLibraries.h"

Sudoku::Sudoku() : Layer("Sudoku"), showDemoWindow(true) {

}

Sudoku::~Sudoku() = default;

void Sudoku::onAttach() {
    Layer::onAttach();
}

void Sudoku::onDetach() {
    Layer::onDetach();
}

void Sudoku::onUpdate(double deltaTimeSeconds) {
    Layer::onUpdate(deltaTimeSeconds);
}

void Sudoku::onGuiRender() {
    Layer::onGuiRender();
    ImGui::ShowDemoWindow(&showDemoWindow);
}

void Sudoku::onEvent(Event &event) {
    Layer::onEvent(event);
}

