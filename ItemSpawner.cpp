#include "ItemSpawner.hpp"

ItemSpawner::ItemSpawner(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    texBungaRaya = LoadTexture("assets/bungaraya.png");
    texTangsi = LoadTexture("assets/talitangsi.png");
    texCoin = LoadTexture("assets/coin.png");
    Reset();
}

void ItemSpawner::Update(float dt) {
    // 
    if (boostTimer > 0.0f) boostTimer -= dt;

    // Bunga Raya Spawning
    bungaSpawnTimer += dt;
    if (bungaSpawnTimer > 15.0f) {
        bungaSpawnTimer = 0.0f;
        bungaItems.push_back(std::make_unique<BungaRaya>(screenWidth, texBungaRaya));
    }

    // Tali Tangsi Spawning
    tangsiSpawnTimer += dt;
    if (tangsiSpawnTimer > 15.0f) {
        tangsiSpawnTimer = 0.0f;
        tangsiItems.push_back(std::make_unique<TaliTangsi>(screenWidth, texTangsi));
    }

    // Coin Spawning
    coinSpawnTimer += dt;
    if (coinSpawnTimer > 2.5f) {
        coinSpawnTimer = 0.0f;
        coinItems.push_back(std::make_unique<Coin>(screenWidth, texCoin));
    }

    // Update Bunga Raya
    for (auto it = bungaItems.begin(); it != bungaItems.end(); ) {
        (*it)->Update(dt);
        if ((*it)->pos.y > screenHeight + (*it)->radius) it = bungaItems.erase(it);
        else ++it;
    }

    // Update Tali Tangsi
    for (auto it = tangsiItems.begin(); it != tangsiItems.end(); ) {
        (*it)->Update(dt);
        if ((*it)->pos.y > screenHeight + (*it)->radius) it = tangsiItems.erase(it);
        else ++it;
    }

    // Update Coin
    for (auto it = coinItems.begin(); it != coinItems.end(); ) {
        (*it)->Update(dt);
        if ((*it)->pos.y > screenHeight + (*it)->radius) it = coinItems.erase(it);
        else ++it;
    }
}

void ItemSpawner::Draw() {
    for (const auto& item : bungaItems) item->Draw();
    for (const auto& item : tangsiItems) item->Draw();
    for (const auto& item : coinItems) item->Draw();
}

int ItemSpawner::CheckBungaCollisions(Vector2 wauPos, float wauRadius, Vector2 kidHitboxPos, float kidRadius) {
    bool collected = false;
    for (auto it = bungaItems.begin(); it != bungaItems.end(); ) {
        if ((*it)->CheckCollision(wauPos, wauRadius) || (*it)->CheckCollision(kidHitboxPos, kidRadius)) {
            boostTimer = 5.0f; // Start 5s boost window
            collected = true;
            it = bungaItems.erase(it);
        } else {
            ++it;
        }
    }
    return collected;
}

int ItemSpawner::CheckTangsiCollisions(Vector2 wauPos, float wauRadius, Vector2 kidHitboxPos, float kidRadius) {
    int tangsiCollected = 0;
    for (auto it = tangsiItems.begin(); it != tangsiItems.end(); ) {
        if ((*it)->CheckCollision(wauPos, wauRadius) || (*it)->CheckCollision(kidHitboxPos, kidRadius)) {
            tangsiCollected += 1;
            it = tangsiItems.erase(it);
        } else ++it;
    }
    return tangsiCollected;
}

int ItemSpawner::CheckCoinCollisions(Vector2 wauPos, float wauRadius, Vector2 kidHitboxPos, float kidRadius) {
    int coinsCollected = 0;
    for (auto it = coinItems.begin(); it != coinItems.end(); ) {
        if ((*it)->CheckCollision(wauPos, wauRadius) || (*it)->CheckCollision(kidHitboxPos, kidRadius)) {
            coinsCollected += 1;
            it = coinItems.erase(it);
        } else ++it;
    }
    return coinsCollected;
}

bool ItemSpawner::IsBoostActive() const {
    return boostTimer > 0.0f;
}

void ItemSpawner::Reset() {
    bungaItems.clear();
    tangsiItems.clear();
    coinItems.clear();
    bungaSpawnTimer = 0.0f;
    tangsiSpawnTimer = 0.0f;
    coinSpawnTimer = 0.0f;
    boostTimer = 0.0f;
}

void ItemSpawner::Unload() {
    UnloadTexture(texBungaRaya);
    UnloadTexture(texTangsi);
    UnloadTexture(texCoin);
}