#include "audio.h"

#include <math.h>
#include <string>
#include <time.h>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

Mix_Chunk* fxLaunch;
Mix_Chunk* fxClick;
Mix_Chunk* fxDeath;
Mix_Chunk* fxCoin;
Mix_Chunk* fxSplash;
Mix_Chunk* fxSelect;

Audio::Audio() {
    audioLaunch = "resources/launch.wav";
    audioClick = "resources/click.wav";
    audioDeath = "resources/die.wav";
    audioCoin = "resources/coin.wav";
    audioSplash = "resources/splash.wav";
    audioSelect = "resources/select.wav";
}

void Audio::setupAudio(){
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
    //SetMasterVolume(0.3f);
    fxLaunch = Mix_LoadWAV(audioLaunch); 
    fxClick = Mix_LoadWAV(audioClick); 
    fxDeath = Mix_LoadWAV(audioDeath);
    fxCoin = Mix_LoadWAV(audioCoin);
    fxSplash = Mix_LoadWAV(audioSplash);
    fxSelect = Mix_LoadWAV(audioSelect);
}

void Audio::freeMemory(){
    Mix_FreeChunk(fxClick);
    Mix_FreeChunk(fxLaunch);
    Mix_FreeChunk(fxDeath);
    Mix_FreeChunk(fxCoin);
    Mix_FreeChunk(fxSplash);
    Mix_FreeChunk(fxSelect);
    Mix_CloseAudio();
}
    
void Audio::playLaunch(){
    Mix_PlayChannel(-1, fxLaunch, 0);
}

void Audio::playClick(){
    Mix_PlayChannel(-1, fxClick, 0);
}

void Audio::playDeath(){
    Mix_PlayChannel(-1, fxDeath, 0);
}

void Audio::playCoin(){
    Mix_PlayChannel(-1, fxCoin, 0);
}

void Audio::playSplash(){
    Mix_PlayChannel(-1, fxSplash, 0);
}

void Audio::playSelect(){
    Mix_PlayChannel(-1, fxSelect, 0);
}