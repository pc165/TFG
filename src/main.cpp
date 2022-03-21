#include "game/Sudoku.h"
#include "engine/App.h"
#include "engine/Logger.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);
    App app;
    Sudoku game;
    app.pushLayer(reinterpret_cast<Layer *>(&game));
    app.run();
    return 0;
}
