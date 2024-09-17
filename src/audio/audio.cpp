#include "audio.hpp"

#include <iostream>

Audio::Audio(const int channels, const int chunksize) {
    /* Open Audio */
    int open_audio_status = Mix_OpenAudio(
        MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, channels, chunksize);

    if (open_audio_status == -1) {
        std::cout << "Mix_OpenAudio: " << Mix_GetError() << std::endl;
    }
}

void Audio::LoadMusic(const char *music_path) {
    /* Load music */
    music = Mix_LoadMUS(music_path);
}

void Audio::PlayMusic() {
    /* Play music */
    int music_status = Mix_PlayMusic(music, -1);

    if (music_status == -1) {
        std::cout << "Mix_PlayMusic: " << Mix_GetError() << std::endl;
    }
}

void Audio::ChangeVolume(const int music_volume) {
    /* Adjust music volume */
    Mix_VolumeMusic(music_volume);
}

void Audio::FreeResources() {
    /* Free audio resources */
    Mix_FreeMusic(music);
    Mix_CloseAudio();
}
