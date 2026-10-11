#pragma once
#include "raylib.h"
#include "GameState.hpp"
#include "Background.hpp"
#include "WauBulan.hpp"
#include "SwingingKid.hpp"
#include "ShopManager.hpp"
#include "AudioManager.hpp"

class MenuSystem {
public:
    static GameState Update(float dt, int screenWidth, int screenHeight, Vector2 virtualMousePos,
                            float& menuIntroTimer, bool& introFinished, Background& menuBg,
                            WauBulan& wau, SwingingKid& kid, float& currentStringLength,
                            ShopManager& shopManager, AudioManager& audio);
};