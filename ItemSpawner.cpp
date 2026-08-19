#include "ItemSpawner.hpp"

ItemSpawner::ItemSpawner(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    texBungaRaya = LoadTexture("assets/bungaraya.png");
    Reset();
}

void ItemSpawner::Update(float dt) {
    // Handle Boost Timer
    if (boostTimer > 0.0f) {
        boostTimer -= dt;
    }

    // Handle Spawning
    spawnTimer += dt;
    if (spawnTimer > 10.0f) {
        spawnTimer = 0.0f;
        items.push_back(std::make_unique<BungaRaya>(screenWidth, texBungaRaya));
    }

    // Update active items and clean up off-screen ones
    for (auto it = items.begin(); it != items.end(); ) {
        (*it)->Update(dt);
        if ((*it)->pos.y > screenHeight + (*it)->radius) {
            it = items.erase(it);
        } else {
            ++it;
        }
    }
}

void ItemSpawner::Draw() {
    for (const auto& item : items) {
        item->Draw();
    }
}

// Returns the amount of altitude gained this frame from collections
int ItemSpawner::CheckCollisions(Vector2 wauPos, float wauRadius, Vector2 kidHitboxPos, float kidRadius) {
    int altitudeBoost = 0;
    for (auto it = items.begin(); it != items.end(); ) {
        if ((*it)->CheckCollision(wauPos, wauRadius) || (*it)->CheckCollision(kidHitboxPos, kidRadius)) {
            altitudeBoost += 50; 
            boostTimer = 5.0f;  
            it = items.erase(it); 
        } else {
            ++it;
        }
    }
    return altitudeBoost;
}

bool ItemSpawner::IsBoostActive() const {
    return boostTimer > 0.0f;
}

void ItemSpawner::Reset() {
    items.clear();
    spawnTimer = 0.0f;
    boostTimer = 0.0f;
}

void ItemSpawner::Unload() {
    UnloadTexture(texBungaRaya);
}