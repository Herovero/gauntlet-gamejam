#include "CollisionManager.hpp"

bool CollisionManager::CheckPlayerCollisions(const WauBulan& wau, const SwingingKid& kid, const ObstacleSpawner& spawner) {
    for (const auto& obs : spawner.obstacles) {
        if (obs->CheckCollision(wau.pos, wau.radius)) return true;
        if (obs->CheckCollision(kid.pos, kid.radius)) return true;
    }
    return false;
}