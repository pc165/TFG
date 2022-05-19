#include "OpenGL.h"
#include "Logger.h"
#include "World.h"
#include "Window.h"

int main(int, char *[]) {
    InitLogger();
    InitWindow("TFG");
    ConfigureEvents(Tools::window);
    {
        World game;
        game.gameLoop();
    }
    DestroyWindow(Tools::window);
    return 0;
}
