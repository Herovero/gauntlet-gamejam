#include "ObstacleSpawner.hpp"
#include "FallingObstacle.hpp"
#include "FlyingObstacle.hpp"
#include "SwayingObstacle.hpp"
#include "GapObstacle.hpp"

ObstacleSpawner::ObstacleSpawner(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->maxObstacles = 12;

    texFalling  = LoadTexture("assets/durian.png");
    texFlying   = LoadTexture("assets/hornbill.png");
    texSwaying  = LoadTexture("assets/leaf.png");
    texGap      = LoadTexture("assets/steel_girder.png");

    Reset();
}

void ObstacleSpawner::SpawnRandomObstacle() {
    bool gapExists = false;
    for (const auto& obs : obstacles) {
        if (obs->IsGapType()) {
            gapExists = true;
            break;
        }
    }

    int maxRandom = gapExists ? 3 : 4; 
    int randomType = GetRandomValue(0, maxRandom);

    if (randomType == 0)      obstacles.push_back(std::make_unique<FallingObstacle>(screenWidth, texFalling));
    else if (randomType == 1) obstacles.push_back(std::make_unique<FlyingObstacle>(screenWidth, screenHeight, true, texFlying));
    else if (randomType == 2) obstacles.push_back(std::make_unique<FlyingObstacle>(screenWidth, screenHeight, false, texFlying));
    else if (randomType == 3) obstacles.push_back(std::make_unique<SwayingObstacle>(screenWidth, texSwaying));
    else if (randomType == 4) obstacles.push_back(std::make_unique<GapObstacle>(screenWidth, texGap));
}

void ObstacleSpawner::Update(float dt) {
    difficultyTimer += dt;
    if (difficultyTimer > 8.0f && obstacles.size() < (size_t)maxObstacles) {
        difficultyTimer = 0.0f;
        SpawnRandomObstacle();
    }

    // Safely iterate through the vector. If an obstacle goes off screen, delete it and spawn a new one.
    for (auto it = obstacles.begin(); it != obstacles.end(); ) {
        (*it)->Update(dt);
        
        if ((*it)->IsOffScreen(screenWidth, screenHeight)) {
            it = obstacles.erase(it); // Erase destroys the object from memory
            SpawnRandomObstacle();    // Spawn a fresh one
        } else {
            ++it;
        }
    }
}

void ObstacleSpawner::Draw() {
    for (const auto& obs : obstacles) {
        obs->Draw();
    }
}

void ObstacleSpawner::Reset() {
    obstacles.clear();
    difficultyTimer = 0.0f;

    for (int i = 0; i < 3; i++) {
        auto obs = std::make_unique<FallingObstacle>(screenWidth, texFalling);
        obs->pos.y -= i * 250.0f;
        obstacles.push_back(std::move(obs));
    }
}

void ObstacleSpawner::Unload() {
    UnloadTexture(texFalling);
    UnloadTexture(texFlying);
    UnloadTexture(texSwaying);
    UnloadTexture(texGap);
}