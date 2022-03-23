#include "Sudoku.h"

Sudoku::Sudoku() : Layer("Sudoku"), showDemoWindow(true) {

}

void Sudoku::onUpdate(double deltaTimeSeconds) {
    c.draw();
}

void Sudoku::onGuiRender() {
    using namespace ImGui;
    namespace fs = std::filesystem;
    if (BeginMainMenuBar()) {
        if (BeginMenu("Game")) {
            if (BeginMenu("Files")) {
                for (auto &p: fs::directory_iterator(fs::current_path())) {
                    if (MenuItem(p.path().filename().c_str())) {
                        LOG_INFO("Load file {}", p.path().string());
                    }
                }
                EndMenu(); // End file
            }
            if (MenuItem("Save", "CTRL+S")) {
                LOG_INFO("Save game");
            }
            if (MenuItem("Quit")) {
                LOG_INFO("Exit");
                exit(0);
            }
            EndMenu(); // Menu
        }
        EndMainMenuBar();
    }
    ShowDemoWindow(&showDemoWindow);
}

void Sudoku::onEvent(Event &event) {

}
