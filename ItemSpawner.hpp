#pragma once
#include "raylib.h"
#include "BungaRaya.hpp"
#include "TaliTangsi.hpp"
#include <vector>
#include <memory>

class ItemSpawner {
public:
    std::vector<std::unique_ptr<BungaRaya>> bungaItems;
    std::vector<std::unique_ptr<TaliTangsi>> tangsiItems;
    
    float bungaSpawnTimer;
    float tangsiSpawnTimer;
    float boostTimer;
        
    int screenWidth;
    int screenHeight;
        
    Texture2D texBungaRaya;
    Texture2D texTangsi;
    
    ItemSpawner(int screenWidth, int screenHeight);
    void Update(float dt);
    void Draw();
    int CheckBungaCollisions(Vector2 wauPos, float wauRadius, Vector2 kidHitboxPos, float kidRadius);
    int CheckTangsiCollisions(Vector2 wauPos, float wauRadius, Vector2 kidHitboxPos, float kidRadius);
    bool IsBoostActive() const;
    void Reset();
    void Unload();
};