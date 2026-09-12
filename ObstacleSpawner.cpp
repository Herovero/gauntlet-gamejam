#include "ObstacleSpawner.hpp"
#include "FallingObstacle.hpp"
#include "FlyingObstacle.hpp"
#include "GapObstacle.hpp"
#include "BouncingObstacle.hpp"

ObstacleSpawner::ObstacleSpawner(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->maxObstacles = 6;

    texFalling  = LoadTexture("assets/durian.png");
    texSwaying  = LoadTexture("assets/gasing.png");
    texGap      = LoadTexture("assets/steelbeam.png");

    for (int i = 0; i < 12; i++) {
        // TextFormat automatically injects the number into the string
        const char* fileName = TextFormat("assets/Hornbill/hornbill_%d.png", i);
        Texture2D frame = LoadTexture(fileName);
        
        GenTextureMipmaps(&frame);
        SetTextureFilter(frame, TEXTURE_FILTER_TRILINEAR);
        
        texFlyingFrames.push_back(frame);
    }

    Reset();
}

void ObstacleSpawner::SpawnRandomObstacle(float currentAltitude, float bgSpeed) {
    bool gapExists = false;
    for (const auto& obs : obstacles) {
        if (obs->IsGapType()) {
            gapExists = true;
            break;
        }
    }

    // Build a pool of allowed obstacle types based on altitude
    std::vector<int> pool;

    if (currentAltitude <= 250.0f) {
        pool.push_back(0); // Falling
    } 
    else if (currentAltitude <= 550.0f) {
        pool.push_back(1); // Flying Left
        pool.push_back(2); // Flying Right
    }
    else if (currentAltitude <= 800.0f) {
        if (!gapExists) {
            pool.push_back(4); // Gap
        } else {
            // Fallback so the pool is never empty if a gap is already falling
            pool.push_back(3);
        }
    }
    else {
        // Endless Mode: Everything is allowed to spawn!
        pool.push_back(0);
        pool.push_back(1);
        pool.push_back(2);
        pool.push_back(3);
        if (!gapExists) pool.push_back(4);
    }

    // Pick a random type from the available pool
    int randomIndex = GetRandomValue(0, pool.size() - 1);
    int randomType = pool[randomIndex];

    // Spawn the chosen obstacle
    if (randomType == 0)      obstacles.push_back(std::make_unique<FallingObstacle>(screenWidth, texFalling));
    else if (randomType == 1) obstacles.push_back(std::make_unique<FlyingObstacle>(screenWidth, screenHeight, true, texFlyingFrames, bgSpeed));
    else if (randomType == 2) obstacles.push_back(std::make_unique<FlyingObstacle>(screenWidth, screenHeight, false, texFlyingFrames, bgSpeed));
    else if (randomType == 3) obstacles.push_back(std::make_unique<BouncingObstacle>(screenWidth, texSwaying));
    else if (randomType == 4) obstacles.push_back(std::make_unique<GapObstacle>(screenWidth, texGap));
}

void ObstacleSpawner::Update(float dt, float currentAltitude, float bgSpeed) {
    if (currentAltitude >= 550 && currentAltitude <= 800.0f) {
        maxObstacles = 4; // Keep the screen less crowded early on
    } else {
        maxObstacles = 6; // Ramp it up to max capacity for Endless Mode
    }

    difficultyTimer += dt;
    if (difficultyTimer > 8.0f && obstacles.size() < (size_t)maxObstacles) {
        difficultyTimer = 0.0f;
        SpawnRandomObstacle(currentAltitude, bgSpeed);
    }

    int obstaclesToReplace = 0;

    // Safely iterate through the vector. If an obstacle goes off screen, delete it and spawn a new one.
    for (auto it = obstacles.begin(); it != obstacles.end(); ) {
        (*it)->SyncBackgroundSpeed(bgSpeed);
        (*it)->Update(dt);
        
        if ((*it)->IsOffScreen(screenWidth, screenHeight)) {
            it = obstacles.erase(it); // Erase destroys the object from memory
            obstaclesToReplace++;    // Spawn a fresh one
        } else {
            ++it;
        }
    }

    for (int i = 0; i < obstaclesToReplace; i++) {
        SpawnRandomObstacle(currentAltitude, bgSpeed);
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

    /*for (int i = 0; i < 3; i++) {
        auto obs = std::make_unique<FallingObstacle>(screenWidth, texFalling);
        obs->pos.y -= i * 250.0f;
        obstacles.push_back(std::move(obs));
    }*/
}

void ObstacleSpawner::Unload() {
    UnloadTexture(texFalling);
    UnloadTexture(texSwaying);
    UnloadTexture(texGap);

    for (auto& frame : texFlyingFrames) {
        UnloadTexture(frame);
    }
}