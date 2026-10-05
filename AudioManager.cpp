#include "AudioManager.hpp"

AudioManager::AudioManager() {
    // Load Music
    gameBgm = LoadMusicStream("assets/bgm.mp3");
    menuBgm = LoadMusicStream("assets/menu_bgm.mp3");
    
    SetMusicVolume(gameBgm, 1.0f);
    SetMusicVolume(menuBgm, 1.0f);
    
    currentBgm = nullptr;

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
    
    SetSoundVolume(sfxHornbill, 0.9f);
    SetSoundVolume(sfxCoin, 0.8f);
    SetSoundVolume(sfxPurchase, 1.0f);
    SetSoundVolume(sfxWind, 0.7f);
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

void AudioManager::StopBGM() {
    if (currentBgm != nullptr) {
        StopMusicStream(*currentBgm);
        currentBgm = nullptr;
    }
}

// SFX Triggers
void AudioManager::PlayHit() { PlaySound(sfxHit); }
void AudioManager::PlayItem() { PlaySound(sfxItem); }
void AudioManager::PlayHornbill() { PlaySound(sfxHornbill); }
void AudioManager::PlayCoin() { PlaySound(sfxCoin); }
void AudioManager::PlayPurchase() { PlaySound(sfxPurchase); }
void AudioManager::PlayWind() { PlaySound(sfxWind); }

void AudioManager::Unload() {
    UnloadMusicStream(gameBgm);
    UnloadMusicStream(menuBgm);
    UnloadSound(sfxHit);
    UnloadSound(sfxItem);
    UnloadSound(sfxHornbill);
    UnloadSound(sfxCoin);
    UnloadSound(sfxPurchase);
    UnloadSound(sfxWind);
}