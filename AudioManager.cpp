#include "AudioManager.hpp"

AudioManager::AudioManager() {
    // Load Music
    gameBgm = LoadMusicStream("assets/bgm.mp3");
    menuBgm = LoadMusicStream("assets/menu_bgm.mp3");
    victoryBgm = LoadMusicStream("assets/victory_bgm.mp3");
    
    SetMusicVolume(gameBgm, 0.5f);
    SetMusicVolume(menuBgm, 1.0f);
    SetMusicVolume(victoryBgm, 0.8f);
    
    currentBgm = nullptr;
    lastHornbillTime = 0.0;

    // Load Existing SFX
    sfxHit = LoadSound("assets/hit.wav");
    sfxItem = LoadSound("assets/pickup.wav");
    SetSoundVolume(sfxHit, 0.8f);
    SetSoundVolume(sfxItem, 0.9f);

    // Load New SFX
    sfxHornbill = LoadSound("assets/hornbill_sfx.wav");
    sfxCoin = LoadSound("assets/coin_sfx.wav");
    sfxPurchase = LoadSound("assets/purchase_sfx.wav");
    sfxWind = LoadSound("assets/wind_sfx.wav");
    sfxSnap = LoadSound("assets/snap_sfx.wav");
    
    SetSoundVolume(sfxHornbill, 0.9f);
    SetSoundVolume(sfxCoin, 0.8f);
    SetSoundVolume(sfxPurchase, 1.0f);
    SetSoundVolume(sfxWind, 1.5f);
    SetSoundVolume(sfxSnap, 1.5f);
}

void AudioManager::Update() {
    if (currentBgm != nullptr) {
        UpdateMusicStream(*currentBgm);
    }
}

void AudioManager::PlayMenuBGM() {
    if (currentBgm != nullptr) StopMusicStream(*currentBgm);
    currentBgm = &menuBgm;
    PlayMusicStream(*currentBgm);
}

void AudioManager::PlayGameBGM() {
    if (currentBgm != nullptr) StopMusicStream(*currentBgm);
    currentBgm = &gameBgm;
    PlayMusicStream(*currentBgm);
}

void AudioManager::PlayVictoryBGM() {
    if (currentBgm != nullptr) StopMusicStream(*currentBgm);
    currentBgm = &victoryBgm;
    PlayMusicStream(*currentBgm);
}

void AudioManager::StopBGM() {
    if (currentBgm != nullptr) {
        StopMusicStream(*currentBgm);
        currentBgm = nullptr;
    }
}

// SFX Triggers
void AudioManager::PlayHit() { PlaySound(sfxHit); }
void AudioManager::PlayItem() { PlaySound(sfxItem); }
void AudioManager::PlayHornbill() { 
    // Check if 2.5 seconds have passed since the last time it played
    if (GetTime() - lastHornbillTime > 2.5) {
        // Randomize the pitch slightly so it doesn't sound repetitive
        SetSoundPitch(sfxHornbill, 0.95f + ((float)GetRandomValue(0, 10) / 100.0f));
        PlaySound(sfxHornbill); 
        
        // Record the exact time we just played it
        lastHornbillTime = GetTime();
    }
}
void AudioManager::PlayCoin() { PlaySound(sfxCoin); }
void AudioManager::PlayPurchase() { PlaySound(sfxPurchase); }
void AudioManager::PlayWind() { PlaySound(sfxWind); }
void AudioManager::StopWind() { StopSound(sfxWind); }
void AudioManager::PlaySnap() { 
    // Randomize pitch between 0.95 and 1.10 for dynamic feedback
    SetSoundPitch(sfxSnap, 0.95f + ((float)GetRandomValue(0, 15) / 100.0f));
    PlaySound(sfxSnap); 
}

void AudioManager::Unload() {
    UnloadMusicStream(gameBgm);
    UnloadMusicStream(menuBgm);
    UnloadMusicStream(victoryBgm);
    UnloadSound(sfxHit);
    UnloadSound(sfxItem);
    UnloadSound(sfxHornbill);
    UnloadSound(sfxCoin);
    UnloadSound(sfxPurchase);
    UnloadSound(sfxWind);
    UnloadSound(sfxSnap);
}