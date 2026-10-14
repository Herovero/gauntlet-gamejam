#pragma once
#include "raylib.h"
#include "GameState.hpp"
#include "Background.hpp"
#include "WauBulan.hpp"
#include "SwingingKid.hpp"
#include "ShopManager.hpp"
#include "CoinManager.hpp"
#include "WindForce.hpp"
#include "KipasSatay.hpp"
#include "ObstacleSpawner.hpp"
#include "ItemSpawner.hpp"
#include "ParticleManager.hpp"
#include "ScoreManager.hpp"

class RenderSystem {
public:
    static void Draw(GameState gameState, int screenWidth, int screenHeight, Vector2 virtualMousePos,
                     bool introFinished, Texture2D gameTitle, Texture2D frozenBg,
                     Background& bg, Background& menuBg, WauBulan& wau, SwingingKid& kid,
                     ShopManager& shopManager, CoinManager& coinManager, WindForce& wind,
                     KipasSatay& kipas, ObstacleSpawner& spawner, ItemSpawner& itemSpawner,
                     ParticleManager& particleManager, ScoreManager& scoreManager, 
                     Texture2D texGithub, Texture2D tex42KL, Texture2D texLarian, Texture2D bgNight);
};