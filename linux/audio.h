#ifndef AUDIO_H
#define AUDIO_H

class Audio {
private:
    char* audioLaunch;
    char* audioClick;
    char* audioDeath;
    char* audioCoin;
    char* audioSplash;
    char* audioSelect;
    
public:
    Audio();
    void setupAudio();
    void freeMemory();
    void playLaunch();
    void playClick();
    void playDeath();
    void playCoin();
    void playSplash();
    void playSelect();
};
#endif