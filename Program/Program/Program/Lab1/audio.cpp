#include "audio.h"
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>
#include <cassert>
#include <iostream>
#include <string>

/*

audio::audio()
{
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096;

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init audio: %s", Mix_GetError());
        exit(-1);
    }
}

audio::~audio()
{
    SDL_Quit();
}

 void audio::addSoundEffect(const char* path)
{
    Mix_Chunk* tmpChunk = Mix_LoadWAV(path);

    if (tmpChunk != nullptr)
    {
        mSoundEffectBank.push_back(tmpChunk);
        std::cout << (mSoundEffectBank.size() - 1) << " Sound is Ready, path: " << path << '\n';
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init audio: %s", Mix_GetError());
    }
}

 void audio::playSoundEffect(const int which) const
 {
     if (which > mSoundEffectBank.size() - 1)
     {
         std::cout << "Sound out of range.\n";
         return;
     }

     Mix_PlayChannel(-1, mSoundEffectBank[which], 0);

     std::cout << "Played Sound: " << which << '\n';
 }

 */
