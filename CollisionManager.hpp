#pragma once
#include "WauBulan.hpp"
#include "SwingingKid.hpp"
#include "ObstacleSpawner.hpp"

class CollisionManager {
public:
    // Returns true if either the Wau Bulan or the Swinging Kid hits any obstacle
    // "static" is used so that we don't need to construct dummy object (e.g, Collision manager; manager.CheckPlayerCollisions(...))
    static bool CheckPlayerCollisions(const WauBulan& wau, const SwingingKid& kid, const ObstacleSpawner& spawner);
};