#pragma once
#include "raylib.h"

class AudioManager {
private:
    Music menuBgm;
    Music gameBgm;
    
    Sound sfxHit;
    Sound sfxItem;
    Sound sfxHornbill;
    Sound sfxCoin;
    Sound sfxPurchase;
    Sound sfxWind;

    Music* currentBgm;

public:
    AudioManager();
    
    void Update(); 
    
    void PlayMenuBGM();
    void PlayGameBGM();
    void StopBGM();

    void PlayHit();
    void PlayItem();
    void PlayHornbill();
    void PlayCoin();
    void PlayPurchase();
    void PlayWind();

    void Unload();
};