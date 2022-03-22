#include "game/Sudoku.h"
#include "engine/App.h"


int main(int , char *[]) {
    App app;
    app.pushLayer(reinterpret_cast<Layer *>(new Sudoku));
    app.run();
    return 0;
}
