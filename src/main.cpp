#include "Utils.h"
#include "World.h"

int main(int, char *[]) {
    tfg::InitLogger();
    tfg::InitWindow("TFG");
    tfg::ConfigureEvents();
    {
        World game;
        game.gameLoop();
    }
    tfg::DestroyWindow();
    return 0;
}
