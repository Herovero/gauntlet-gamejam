#include "GameManager.hpp"

void GameManager::ResetGame(int screenWidth, int screenHeight, WauBulan& wau, SwingingKid& kid, 
                            Background& bg, ObstacleSpawner& spawner, ItemSpawner& itemSpawner, 
                            ScoreManager& scoreManager, CoinManager& coinManager, 
                            WindForce& wind, KipasSatay& kipas) {
    bg.Reset();
    spawner.Reset();
    itemSpawner.Reset();
    scoreManager.Reset();
    coinManager.Reset();
    wind.Reset();
    kipas.Reset();

    // Reset entities to main menu positions
    wau.pos = { (float)screenWidth / 2.0f, (float)screenHeight - 600.0f };
    kid.pos = { (float)screenWidth / 2.0f, (float)screenHeight - 50.0f };
    kid.velocity = { 0.0f, 0.0f };
    kid.isDetached = false;
    kid.isOnGround = true;
}