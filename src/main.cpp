#include "Utils.h"
#include "World.h"
#include "Audio.h"

int main() {
    tfg::InitLogger();

    char buffer[20] = {0};
    for (int i = 0; i < 10; i++) {
        sprintf(buffer, "res/%d.wav", i);
        if (tfg::LoadAudioFiles(buffer))
            return 1;
    }

    tfg::InitAudio();
    tfg::InitWindow("TFG", 1080, 720, false);
    tfg::ConfigureEvents();
    {
        World game;
        game.gameLoop();
    }
    tfg::DestroyWindow();
    tfg::DestroyAudio();
    return 0;
}
