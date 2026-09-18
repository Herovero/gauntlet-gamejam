#include "CollisionManager.hpp"

bool CollisionManager::CheckPlayerCollisions(const WauBulan& wau, const SwingingKid& kid, const ObstacleSpawner& spawner) {
    for (const auto& obs : spawner.obstacles) {
        if (obs->CheckCollision(wau.pos, wau.radius)) return true;
        if (obs->CheckCollision(kid.pos, kid.radius)) return true;
    }
    return false;
}

void CollisionManager::HandleItemCollections(WauBulan& wau, SwingingKid& kid, ItemSpawner& itemSpawner, 
    ScoreManager& scoreManager, CoinManager& coinManager, Sound sfxItem) {
        
    if (kid.isDetached) return;

    Vector2 kidHitboxPos = { kid.pos.x - 15.0f, kid.pos.y };

    int boost = itemSpawner.CheckBungaCollisions(wau.pos, wau.radius, kidHitboxPos, kid.radius);
    if (boost > 0) { scoreManager.currentAltitude += boost; PlaySound(sfxItem); }

    int extraStrings = itemSpawner.CheckTangsiCollisions(wau.pos, wau.radius, kidHitboxPos, kid.radius);
    if (extraStrings > 0) { scoreManager.stringCharges += extraStrings; PlaySound(sfxItem); }

    int collectedCoins = itemSpawner.CheckCoinCollisions(wau.pos, wau.radius, kidHitboxPos, kid.radius);
    if (collectedCoins > 0) { coinManager.AddCoins(collectedCoins); PlaySound(sfxItem); }
}