#pragma once
#include "raylib.h"
#include "Obstacle.hpp"
#include <vector>

class ObstacleSpawner {
public:
    std::vector<Obstacle> obstacles;
    float difficultyTimer;
    int maxObstacles;
    int screenWidth;
    int screenHeight;

    ObstacleSpawner(int screenWidth, int screenHeight);
    void Update(float dt);
    void Draw();
    void Reset();
};