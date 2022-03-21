#include "game/Sudoku.h"
#include "engine/App.h"


int main(int argc, char *argv[]) {
    App app;
    app.pushLayer(reinterpret_cast<Layer *>(new Sudoku));
    app.run();
    return 0;
}
