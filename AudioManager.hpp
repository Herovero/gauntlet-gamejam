#pragma once
#include "raylib.h"

class AudioManager {
private:
    Music menuBgm;
    Music gameBgm;
    Music victoryBgm;
    
    Sound sfxHit;
    Sound sfxItem;
    Sound sfxHornbill;
    Sound sfxCoin;
    Sound sfxPurchase;
    Sound sfxWind;
    Sound sfxSnap;

    Music* currentBgm;

    double lastHornbillTime;

public:
    AudioManager();
    
    void Update(); 
    
    void PlayMenuBGM();
    void PlayGameBGM();
    void PlayVictoryBGM();
    void StopBGM();

    void PlayHit();
    void PlayItem();
    void PlayHornbill();
    void PlayCoin();
    void PlayPurchase();
    void PlayWind();
    void StopWind();
    void PlaySnap();

    void Unload();
};