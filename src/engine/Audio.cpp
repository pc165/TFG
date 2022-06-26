#include "Audio.h"
#include "Logger.h"
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#include <xaudio2.h>
#include <winerror.h>
#include <mmeapi.h>
#include <winnt.h>
#include <fileapi.h>

#define SAMPLE_RATE 44100
#define TOTAL_CHANNELS 2
#define MAX_AUDIOS 10
#define PACK( __Declaration__ ) __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
PACK(struct WavFile {
    // RIFF Header
    int8_t riff_header[4]; // Contains "RIFF"
    int32_t wav_size;      // Size of the wav portion of the file, which follows the first 8 bytes. File size - 8
    int8_t wave_header[4]; // Contains "WAVE"

    // Format Header
    int8_t fmt_header[4];   // Contains "fmt " (includes trailing space)
    int32_t fmt_chunk_size; // Should be 16 for PCM
    int16_t format;         // Should be 1 for PCM. 3 for IEEE Float
    int16_t num_channels;
    int32_t sample_rate;
    int32_t byte_rate;        // Number of bytes per second. sample_rate * num_channels * Bytes Per Sample
    int16_t sample_alignment; // num_channels * Bytes Per Sample
    int16_t bit_depth;        // Number of bits per sample

    // Data
    int8_t data_header[4]; // Contains "data"
    int32_t data_bytes;    // Number of bytes in data. Number of samples * num_channels * sample byte size
    void *bytes;           // Remainder of wave file is bytes
});

typedef XAUDIO2_BUFFER AudioData;
static AudioData audios[MAX_AUDIOS];
static int audio_size = 0;

int tfg::LoadAudioFiles(char const *path) {
    HANDLE hFile = CreateFileA(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if(INVALID_HANDLE_VALUE == hFile) {
        LOG_ERROR("Cannot load {}", path);
        return -1;
    }

    if(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN)) {
        LOG_ERROR("Cannot set cursor");
        return -1;
    }

    LARGE_INTEGER size{ 0 };
    if(!GetFileSizeEx(hFile, &size)) {
        LOG_ERROR("Cannot get file size");
        return -1;
    }

    LOG_INFO("File size is {}", size.LowPart);

    void *data = (void *)malloc(size.LowPart);
    DWORD bytesRead = 0;

    if(!ReadFile(hFile, data, size.LowPart, &bytesRead, 0)) {
        LOG_ERROR("Cannot read file");
        return -1;
    }

    CloseHandle(hFile);

    auto wav_file = static_cast<struct WavFile *>(data);
    if(wav_file->sample_rate != SAMPLE_RATE ||
       wav_file->num_channels != TOTAL_CHANNELS ||
       wav_file->bit_depth != 16 ||
       wav_file->format != 1)
        LOG_ERROR("Couldn't load audio file");

    assert(wav_file->sample_rate == SAMPLE_RATE);
    assert(wav_file->num_channels == TOTAL_CHANNELS);
    assert(wav_file->bit_depth == 16); // 16
    assert(wav_file->format == 1);     // PCM
    assert(wav_file->wave_header[0] == 'W');
    assert(wav_file->wave_header[1] == 'A');
    assert(wav_file->wave_header[2] == 'V');
    assert(wav_file->wave_header[3] == 'E');
    auto &audio = audios[audio_size++];

    audio.pAudioData = static_cast<BYTE *>(malloc(wav_file->data_bytes));
    audio.AudioBytes = wav_file->data_bytes;
    audio.Flags = XAUDIO2_END_OF_STREAM;

    assert(audio.pAudioData != nullptr);

    memcpy((void *)audio.pAudioData, &wav_file->bytes, wav_file->data_bytes);

    LOG_INFO("Created {}", path);

    free(wav_file);
    return 0;
}

static IXAudio2 *pXAudio2 = nullptr;
static IXAudio2SourceVoice *pSourceVoice = nullptr;

int tfg::InitAudio() {
    HRESULT hr;
    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if(FAILED(hr))
        return hr;


    if(FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_USE_DEFAULT_PROCESSOR)))
        return hr;

    IXAudio2MasteringVoice *pMasterVoice = nullptr;
    if(FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
        return hr;
    WAVEFORMATEX wfx;

    wfx.nChannels = TOTAL_CHANNELS;
    wfx.nSamplesPerSec = SAMPLE_RATE;
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nAvgBytesPerSec = TOTAL_CHANNELS * SAMPLE_RATE * 2;
    wfx.cbSize = 0;
    wfx.nBlockAlign = 4;
    wfx.wBitsPerSample = 16;


    hr = pXAudio2->CreateSourceVoice(&pSourceVoice, &wfx);
    if(FAILED(hr)) {
        HRESULT_FROM_WIN32(GetLastError());
        LOG_ERROR("Cannot create audio source");
        return -1;
    }

    return 0;
}

void tfg::PlayNumberAudio(int number) {
    LOG_DEBUG("Play {}", number);
    assert(number < audio_size);
    if(!(number < audio_size)) {
        LOG_ERROR("Out of range");
        return;
    }

    if(!pSourceVoice) {
        LOG_ERROR("Source is nullptr");
        return;
    }

    XAUDIO2_VOICE_STATE state = {};
    pSourceVoice->GetState(&state);

    if(state.BuffersQueued != 0) {
        return;
    }

    if(FAILED(pSourceVoice->SubmitSourceBuffer(&audios[number]))) {
        LOG_ERROR("Cannot play audio");
        return;
    }

    if(FAILED(pSourceVoice->Start(0))) {
        LOG_ERROR("Cannot play audio");
        return;
    }

}

void tfg::DestroyAudio() {
    if(pSourceVoice) {
        pSourceVoice->DestroyVoice();
    }

    if(pXAudio2) {
        pXAudio2->StopEngine();
        pXAudio2->Release();
    }
}

#elif __unix__

#include <cmath>
#include <csignal>
#include <thread>

#include <fcntl.h>
#include <pipewire/pipewire.h>
#include <spa/param/audio/format-utils.h>
#include <spa/param/audio/format.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define SAMPLE_RATE 44100
#define TOTAL_CHANNELS 2
#define MAX_AUDIOS 10

// WAV header spec information:
//https://gist.github.com/Jon-Schneider/8b7c53d27a7a13346a643dac9c19d34f
//https://web.archive.org/web/20140327141505/https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
//http://www.topherlee.com/software/pcm-tut-wavformat.html
struct __attribute__((packed)) WavFile {
    // RIFF Header
    int8_t riff_header[4]; // Contains "RIFF"
    int32_t wav_size;      // Size of the wav portion of the file, which follows the first 8 bytes. File size - 8
    int8_t wave_header[4]; // Contains "WAVE"

    // Format Header
    int8_t fmt_header[4];   // Contains "fmt " (includes trailing space)
    int32_t fmt_chunk_size; // Should be 16 for PCM
    int16_t format;         // Should be 1 for PCM. 3 for IEEE Float
    int16_t num_channels;
    int32_t sample_rate;
    int32_t byte_rate;        // Number of bytes per second. sample_rate * num_channels * Bytes Per Sample
    int16_t sample_alignment; // num_channels * Bytes Per Sample
    int16_t bit_depth;        // Number of bits per sample

    // Data
    int8_t data_header[4]; // Contains "data"
    int32_t data_bytes;    // Number of bytes in data. Number of samples * num_channels * sample byte size
    void *bytes;           // Remainder of wave file is bytes
};

struct AudioData {
    uint8_t *buffer;
    uint32_t stride;
    uint32_t total_frames;
    uint32_t frames_played;
};

struct UserData {
    struct pw_main_loop *loop;
    struct pw_stream *stream;
    AudioData *audio;
    bool finished;
    bool quit;
};

struct ThreadData {
    pthread_mutex_t lock;
    pthread_t handle;
};

static struct AudioData audios[MAX_AUDIOS];
static int audio_size = 0;
static struct UserData user_data = { .loop = nullptr, .stream = nullptr, .audio = nullptr, .finished = true, .quit = false };
static struct ThreadData thread_data;

//https://docs.pipewire.org/tutorial4_8c-example.html

static void on_process(void *dataPtr) {
    auto *data = static_cast<struct UserData *>(dataPtr);
    pthread_mutex_lock(&thread_data.lock);

    if(data->quit) {
        LOG_DEBUG("Event audio quit");
        pthread_mutex_unlock(&thread_data.lock);
        pw_main_loop_quit(data->loop);
        return;
    }

    if(data->finished || data->audio == nullptr) {
        pthread_mutex_unlock(&thread_data.lock);
        return;
    }

    struct pw_buffer *pw_buffer = pw_stream_dequeue_buffer(data->stream);
    if(pw_buffer == nullptr) {
        pthread_mutex_unlock(&thread_data.lock);
        LOG_ERROR("Out of buffers");
        return;
    }

    struct spa_buffer *spa_buffer = pw_buffer->buffer;
    auto *buffer = static_cast<int16_t *>(spa_buffer->datas[0].data);

    if(buffer == nullptr) {
        pthread_mutex_unlock(&thread_data.lock);
        return;
    }

    memset(buffer, 0, spa_buffer->datas[0].maxsize);

    AudioData *audio = data->audio;

    int stride = audio->stride;
    int n_frames = spa_buffer->datas[0].maxsize / stride;
    int remaining_frames = audio->total_frames - audio->frames_played;

    if(n_frames > remaining_frames)
        n_frames = remaining_frames;

    for(int i = 0; i < n_frames * TOTAL_CHANNELS; i++) {
        int32_t sample = reinterpret_cast<int16_t *>(audio->buffer)[audio->frames_played * TOTAL_CHANNELS + i];

        if(sample < INT16_MIN) sample = INT16_MIN;
        if(sample > INT16_MAX) sample = INT16_MAX;

        *buffer++ = sample;
    }

    audio->frames_played += n_frames;
    if(audio->frames_played >= audio->total_frames) {
        audio->frames_played = 0;
        data->finished = true;
    }

    pthread_mutex_unlock(&thread_data.lock);

    pw_buffer->size = n_frames;
    spa_buffer->datas[0].chunk->offset = 0;
    spa_buffer->datas[0].chunk->stride = stride;
    spa_buffer->datas[0].chunk->size = n_frames * stride;
    pw_stream_queue_buffer(data->stream, pw_buffer);
}

static const struct pw_stream_events stream_events = {
    .version = PW_VERSION_STREAM_EVENTS,
    .process = on_process,
};

static void *audio_main_thread(void *) {
    LOG_INFO("Audio thread main");
    const struct spa_pod *params[1];
    uint8_t buffer[1024];
    struct spa_pod_builder b = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));

    pw_init(nullptr, nullptr);

    /* make a main loop. If you already have another main loop, you can add
     * the fd of this pipewire mainloop to it. */
    user_data.loop = pw_main_loop_new(nullptr);

    /* Create a simple stream, the simple stream manages the core and remote
     * objects for you if you don't need to deal with them.
     *
     * If you plan to autoconnect your stream, you need to provide at least
     * media, category and role properties.
     *
     * Pass your events and a user_data pointer as the last arguments. This
     * will inform you about the stream state. The most important event
     * you need to listen to is the process event where you need to produce
     * the data.
     */
    user_data.stream = pw_stream_new_simple(
        pw_main_loop_get_loop(user_data.loop),
        "audio-src",
        pw_properties_new(
            PW_KEY_MEDIA_TYPE, "Audio",
            PW_KEY_MEDIA_CATEGORY, "Playback",
            PW_KEY_MEDIA_ROLE, "Music",
            nullptr),
        &stream_events,
        &user_data);

    /* Make one parameter with the supported formats. The SPA_PARAM_EnumFormat
     * id means that this is a format enumeration (of 1 value). */
    struct spa_audio_info_raw info = {
        .format = SPA_AUDIO_FORMAT_S16_LE,
        .rate = SAMPLE_RATE,
        .channels = TOTAL_CHANNELS };

    params[0] = spa_format_audio_raw_build(&b, SPA_PARAM_EnumFormat, &info);

    /* Now connect this stream. We ask that our process function is
     * called in a realtime thread. */
    pw_stream_connect(user_data.stream,
                      PW_DIRECTION_OUTPUT,
                      PW_ID_ANY,
                      static_cast<pw_stream_flags>(PW_STREAM_FLAG_AUTOCONNECT |
                                                   PW_STREAM_FLAG_MAP_BUFFERS |
                                                   PW_STREAM_FLAG_RT_PROCESS),
                      params, 1);

    /* and wait while we let things run */
    pw_main_loop_run(user_data.loop);

    pw_stream_destroy(user_data.stream);
    pw_main_loop_destroy(user_data.loop);
    pw_deinit();
    LOG_DEBUG("Audio thread exit");
    return nullptr;
}

int tfg::LoadAudioFiles(char const *path) {
    int fd = open(path, O_RDONLY);
    LOG_INFO("Load audio {}", path);

    if(audio_size >= MAX_AUDIOS) {
        LOG_ERROR("Too manu audios");
        return -1;
    }

    if(fd == -1) {
        LOG_ERROR("Cannot open file {}", path);
        return -1;
    }

    int size = lseek(fd, 0, SEEK_END);

    if(size == -1) {
        LOG_ERROR("Cannot find file size");
        return -1;
    }

    if(lseek(fd, 0, SEEK_SET) == -1) {
        LOG_ERROR("Cannot reset cursor");
        return -1;
    }

    void *data = (void *)malloc(size);
    if(read(fd, data, size) == -1) {
        LOG_ERROR("Cannot read");
        return -1;
    }

    close(fd);

    auto wav_file = static_cast<struct WavFile *>(data);
    if(wav_file->sample_rate != SAMPLE_RATE ||
       wav_file->num_channels != TOTAL_CHANNELS ||
       wav_file->bit_depth != 16 ||
       wav_file->format != 1)
        LOG_ERROR("Couldn't load audio file");

    assert(wav_file->sample_rate == SAMPLE_RATE);
    assert(wav_file->num_channels == TOTAL_CHANNELS);
    assert(wav_file->bit_depth == 16); // 16
    assert(wav_file->format == 1);     // PCM
    assert(wav_file->wave_header[0] == 'W');
    assert(wav_file->wave_header[1] == 'A');
    assert(wav_file->wave_header[2] == 'V');
    assert(wav_file->wave_header[3] == 'E');

    auto &audio = audios[audio_size++];

    audio.buffer = static_cast<uint8_t *>(malloc(wav_file->data_bytes));
    audio.total_frames = wav_file->data_bytes / wav_file->sample_alignment;
    audio.stride = wav_file->num_channels * sizeof(int16_t);
    audio.frames_played = 0;

    assert(audio.buffer != nullptr);

    memcpy(audio.buffer, &wav_file->bytes, wav_file->data_bytes);

    LOG_INFO("Created {}", path);

    free(wav_file);
    return 0;
}

int tfg::InitAudio() {
    if(pthread_mutex_init(&thread_data.lock, nullptr)) {
        LOG_ERROR("Cannot create mutex");
        return -1;
    }

    if(pthread_create(&thread_data.handle, nullptr, audio_main_thread, nullptr)) {
        LOG_ERROR("Cannot create thread");
        return -1;
    }

    LOG_INFO("Audio Thread created");

    return 0;
}

void tfg::PlayNumberAudio(int number) {
    LOG_DEBUG("Play {}", number);
    assert(number < audio_size);

    pthread_mutex_lock(&thread_data.lock);

    user_data.audio = &audios[number];
    user_data.audio->frames_played = 0;
    user_data.finished = false;

    pthread_mutex_unlock(&thread_data.lock);
}

void tfg::DestroyAudio() {
    LOG_DEBUG("Destroy audio");

    for(int i = 0; i < audio_size; ++i) {
        LOG_INFO("Free audio {}", i);
        free(audios[i].buffer);
    }

    pthread_mutex_lock(&thread_data.lock);
    user_data.audio = nullptr;
    user_data.finished = true;
    user_data.quit = true;
    pthread_mutex_unlock(&thread_data.lock);

    pthread_join(thread_data.handle, nullptr);
}

#endif