#pragma once
#include "raylib.h"
#include "BungaRaya.hpp"
#include <vector>
#include <memory>

class ItemSpawner {
public:
    std::vector<std::unique_ptr<BungaRaya>> items;
    float spawnTimer;
    float boostTimer;
    int screenWidth;
    int screenHeight;
    Texture2D texBungaRaya;

    ItemSpawner(int screenWidth, int screenHeight);
    void Update(float dt);
    void Draw();
    int CheckCollisions(Vector2 wauPos, float wauRadius, Vector2 kidHitboxPos, float kidRadius);
    bool IsBoostActive() const;
    void Reset();
    void Unload();
};