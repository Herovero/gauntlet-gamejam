#pragma once
#include "WauBulan.hpp"
#include "SwingingKid.hpp"
#include "Background.hpp"
#include "ObstacleSpawner.hpp"
#include "ItemSpawner.hpp"
#include "ScoreManager.hpp"
#include "CoinManager.hpp"
#include "WindForce.hpp"
#include "KipasSatay.hpp"

class GameManager {
public:
    static void ResetGame(int screenWidth, int screenHeight, WauBulan& wau, SwingingKid& kid, 
                          Background& bg, ObstacleSpawner& spawner, ItemSpawner& itemSpawner, 
                          ScoreManager& scoreManager, CoinManager& coinManager, 
                          WindForce& wind, KipasSatay& kipas);
};