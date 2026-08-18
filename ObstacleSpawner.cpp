#include "ObstacleSpawner.hpp"

ObstacleSpawner::ObstacleSpawner(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->maxObstacles = 12;
    Reset();
}

void ObstacleSpawner::Update(float dt) {
    // Check if the gap current exists on screen
    bool gapExists = false;
    for (const auto& obs : obstacles) {
        if (obs.type == TYPE_GAP) {
            gapExists = true;
            break;
        }
    }

    // Difficulty Scaling
    difficultyTimer += dt;
    // Check if 8 second of gameplay has passed
    // And check if the amount of obstacles doesn't exceed maximum
    if (difficultyTimer > 8.0f && obstacles.size() < (size_t)maxObstacles) {
        difficultyTimer = 0.0f;
        // Construct a new obstacle object and add to end of vector list
        obstacles.push_back(Obstacle(screenWidth, screenHeight, !gapExists));
    }

    // Loop through every obstacle element to update all active obstacles
    for (auto& obs : obstacles) {
        obs.Update(dt, screenWidth, screenHeight, !gapExists);
    }
}

void ObstacleSpawner::Draw() {
    for (auto& obs : obstacles) {
        obs.Draw();
    }
}

void ObstacleSpawner::Reset() {
    obstacles.clear();
    difficultyTimer = 0.0f;

    // Spawn the first 3 obstacles
    for (int i = 0; i < 3; i++) {
        Obstacle obs(screenWidth, screenHeight, false);
        // Differentiate their initial y position so they don't drop at the exact same time
        obs.rec.y -= i * 250.0f; 
        obstacles.push_back(obs);
    }
}