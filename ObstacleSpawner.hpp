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

    ObstacleSpawner(int screenWidth, int screenHeight);
    void Update(float dt);
    void Draw();
    void Reset();
    
    // Helper function to handle creating the unique_ptrs
    void SpawnRandomObstacle();
};