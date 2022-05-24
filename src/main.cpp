#include "Utils.h"
#include "World.h"

int main(int, char *[]) {
    tfg::InitLogger();
    tfg::InitWindow("TFG");
    tfg::ConfigureEvents(Injector::window);
    {
        World game;
        game.gameLoop();
    }
    tfg::DestroyWindow(Injector::window);
    return 0;
}
