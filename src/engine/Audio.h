#ifndef TFG_AUDIO_H
#define TFG_AUDIO_H


namespace tfg {
    void LoadAudioFiles(char const *path);

    bool InitAudio();

    void PlayNumberAudio(int number);

    void DestroyAudio();
}

#endif //TFG_AUDIO_H
