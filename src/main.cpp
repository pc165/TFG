#include "Logger.h"
#include "World.h"
#include "Window.h"

int main(int, char *[]) {
    InitLogger();
    InitWindow("TFG");
    ConfigureEvents(Injector::window);
    {
        World game;
        game.gameLoop();
    }
    DestroyWindow(Injector::window);
    return 0;
}
