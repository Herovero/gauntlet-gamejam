#pragma once
#include "raylib.h"
#include "GameState.hpp"
#include "ShopManager.hpp"
#include "CoinManager.hpp"
#include "AudioManager.hpp"
#include "WauBulan.hpp"
#include "SwingingKid.hpp"

class ShopSystem {
public:
    static GameState Update(float dt, int screenWidth, Vector2 virtualMousePos,
                            ShopManager& shopManager, CoinManager& coinManager,
                            AudioManager& audio, WauBulan& wau, SwingingKid& kid,
                            float& currentStringLength);
};