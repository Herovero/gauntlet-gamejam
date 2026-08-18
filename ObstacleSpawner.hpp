#pragma once
#include "raylib.h"
#include "Obstacle.hpp"
#include <vector>
#include <memory>

class ObstacleSpawner {
public:
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    float difficultyTimer;
    int maxObstacles;
    int screenWidth;
    int screenHeight;

    Texture2D texFalling;
    Texture2D texFlying;
    Texture2D texSwaying;
    Texture2D texGap;

    ObstacleSpawner(int screenWidth, int screenHeight);
    void Update(float dt);
    void Draw();
    void Reset();
    void Unload();
    void SpawnRandomObstacle();
};