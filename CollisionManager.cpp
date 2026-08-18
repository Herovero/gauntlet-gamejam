#include "CollisionManager.hpp"

bool CollisionManager::CheckPlayerCollisions(const WauBulan& wau, const SwingingKid& kid, const ObstacleSpawner& spawner) {
    for (const auto& obs : spawner.obstacles) {
        // Check collision for the Wau Bulan
        if (CheckCollisionCircleRec(wau.pos, wau.radius, obs.rec)) {
            return true;
        }

        // Check collision for the swinging kid
        if (CheckCollisionCircleRec(kid.pos, kid.radius, obs.rec)) {
            return true;
        }

        if (obs.type == TYPE_GAP) {
            if (CheckCollisionCircleRec(wau.pos, wau.radius, obs.rec2)) return true;
            if (CheckCollisionCircleRec(kid.pos, kid.radius, obs.rec2)) return true;
        }
    }
    return false;
}