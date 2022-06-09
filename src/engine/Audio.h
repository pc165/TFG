#ifndef TFG_AUDIO_H
#define TFG_AUDIO_H


namespace tfg {
    int LoadAudioFiles(char const *path);

    int InitAudio();

    void PlayNumberAudio(int number);

    void DestroyAudio();
}

#endif //TFG_AUDIO_H
