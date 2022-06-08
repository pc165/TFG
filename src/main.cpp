#include "Utils.h"
#include "World.h"
#include "Audio.h"

int main() {
    tfg::LoadAudioFiles("sound0.wav");
    if (tfg::InitAudio()) {
        LOG_INFO("Audio Thread created");
    }
    tfg::InitLogger();
    tfg::InitWindow("TFG");
    tfg::ConfigureEvents();
    {
        World game;
        game.gameLoop();
    }
    tfg::DestroyWindow();
    tfg::DestroyAudio();
    return 0;
}
