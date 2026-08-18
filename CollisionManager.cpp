#include "CollisionManager.hpp"

bool CollisionManager::CheckPlayerCollisions(const WauBulan& wau, const SwingingKid& kid, const ObstacleSpawner& spawner) {
    for (const auto& obs : spawner.obstacles) {
        // obs->CheckCollision automatically routes to GapObstacle's special code if it is a Gap!
        if (obs->CheckCollision(wau.pos, wau.radius)) return true;
        if (obs->CheckCollision(kid.pos, kid.radius)) return true;
    }
    return false;
}