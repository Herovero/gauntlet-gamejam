#pragma once
#include "raylib.h"
#include "GameState.hpp"
#include "Background.hpp"
#include "WauBulan.hpp"
#include "SwingingKid.hpp"
#include "WindForce.hpp"
#include "KipasSatay.hpp"
#include "ScoreManager.hpp"
#include "ObstacleSpawner.hpp"
#include "ItemSpawner.hpp"
#include "ParticleManager.hpp"
#include "CoinManager.hpp"
#include "AudioManager.hpp"
#include "RewindSystem.hpp"

class GameplaySystem {
public:
    static GameState Update(float dt, int screenWidth, int screenHeight, Vector2 virtualMousePos,
                            float& hitStopTimer, Background& bg, WauBulan& wau, SwingingKid& kid,
                            WindForce& wind, KipasSatay& kipas, ScoreManager& scoreManager,
                            ObstacleSpawner& spawner, ItemSpawner& itemSpawner,
                            ParticleManager& particleManager, CoinManager& coinManager,
                            AudioManager& audio, RewindSystem& rewindSystem);
};