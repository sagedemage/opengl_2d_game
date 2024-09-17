#ifndef AUDIO_H
#define AUDIO_H

class Audio {
   public:
    Audio(int channels, int chunksize);  // open audio

    // Methods
    void LoadMusic(const char *music_path);
    void PlayMusic();
    static void ChangeVolume(int music_volume);
    void FreeResources();

   private:
    // Variables
    Mix_Music *music = NULL;
};

#endif  // AUDIO_H
