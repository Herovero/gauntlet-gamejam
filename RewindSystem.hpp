#pragma once
#include "raylib.h"
#include "GameState.hpp"
#include "WauBulan.hpp"
#include "SwingingKid.hpp"
#include "Background.hpp"
#include "ObstacleSpawner.hpp"
#include "ItemSpawner.hpp"
#include "ScoreManager.hpp"
#include "WindForce.hpp"
#include "KipasSatay.hpp"
#include "ShopManager.hpp"
#include "AudioManager.hpp"

class RewindSystem {
private:
    float timer;
    bool skipFall;

public:
    RewindSystem();

    // Triggered the moment the player dies
    void TriggerDeath(float altitude);

    void PrepareRising();
    
    // State update functions return the next GameState
    GameState UpdateWaiting(float dt, Background& bg, WauBulan& wau, ObstacleSpawner& spawner, ItemSpawner& itemSpawner, ScoreManager& scoreManager, AudioManager& audio);
    
    GameState UpdateRewinding(float dt, Background& bg, ScoreManager& scoreManager, WindForce& wind, KipasSatay& kipas, ShopManager& shopManager, WauBulan& wau, SwingingKid& kid, float& currentStringLength, int screenHeight);
    
    GameState UpdateFalling(float dt, WauBulan& wau, SwingingKid& kid, float currentStringLength, int screenHeight);
    
    GameState UpdateRising(float dt, Background& bg, WauBulan& wau, SwingingKid& kid, float currentStringLength, int screenHeight);
};