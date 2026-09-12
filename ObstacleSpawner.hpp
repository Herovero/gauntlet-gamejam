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
    std::vector<Texture2D> texFlyingFrames;
    Texture2D texSwaying;
    Texture2D texGap;

    ObstacleSpawner(int screenWidth, int screenHeight);
    void Update(float dt, float currentAltitude, float bgSpeed);
    void Draw();
    void Reset();
    void Unload();
    void SpawnRandomObstacle(float currentAltitude, float bgSpeed);
};