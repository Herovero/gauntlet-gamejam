#pragma once
#include "raylib.h"
#include "GameState.hpp"
#include "AudioManager.hpp"
#include "WauBulan.hpp"
#include "SwingingKid.hpp"
#include "Background.hpp"
#include "ObstacleSpawner.hpp"
#include "ItemSpawner.hpp"
#include "ScoreManager.hpp"
#include "CoinManager.hpp"
#include "WindForce.hpp"
#include "KipasSatay.hpp"
#include "ShopManager.hpp"
#include "RewindSystem.hpp"

class OverlaySystem {
public:
    static GameState Update(GameState currentState, int screenWidth, int screenHeight, Vector2 virtualMousePos,
                            AudioManager& audio, WauBulan& wau, SwingingKid& kid, Background& bg,
                            ObstacleSpawner& spawner, ItemSpawner& itemSpawner, ScoreManager& scoreManager,
                            CoinManager& coinManager, WindForce& wind, KipasSatay& kipas, ShopManager& shopManager,
                            float& menuIntroTimer, bool& introFinished, float currentStringLength, RewindSystem& rewindSystem);
};