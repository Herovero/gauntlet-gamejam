#include "ItemSpawner.hpp"

ItemSpawner::ItemSpawner(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    texBungaRaya = LoadTexture("assets/bungaraya.png");
    texTangsi = LoadTexture("assets/talitangsi.png");
    texAisKacang = LoadTexture("assets/ais_kacang.png");
    texCoin = LoadTexture("assets/coin.png");
    Reset();
}

void ItemSpawner::Update(float dt, float envDt) {
    if (boostTimer > 0.0f) boostTimer -= dt;
    if (slowMoTimer > 0.0f) slowMoTimer -= dt;

    // Bunga Raya Spawning
    bungaSpawnTimer += envDt;
    if (bungaSpawnTimer > 15.0f) {
        // Reset with a random variance between -3 and 3 seconds
        bungaSpawnTimer = (float)GetRandomValue(-3, 3); 
        bungaItems.push_back(std::make_unique<BungaRaya>(screenWidth, texBungaRaya));
    }

    // Tali Tangsi Spawning
    tangsiSpawnTimer += envDt;
    if (tangsiSpawnTimer > 15.0f) {
        tangsiSpawnTimer = (float)GetRandomValue(-3, 3); 
        tangsiItems.push_back(std::make_unique<TaliTangsi>(screenWidth, texTangsi));
    }

    // Ais Kacang Spawning
    aisKacangSpawnTimer += envDt;
    if (aisKacangSpawnTimer > 25.0f) {
        aisKacangSpawnTimer = (float)GetRandomValue(-5, 5); 
        aisKacangItems.push_back(std::make_unique<AisKacang>(screenWidth, texAisKacang));
    }

    // Coin Spawning
    coinSpawnTimer += envDt;
    if (coinSpawnTimer > 2.5f) {
        coinSpawnTimer = 0.0f;
        coinItems.push_back(std::make_unique<Coin>(screenWidth, texCoin));
    }

    // Update Bunga Raya
    for (auto it = bungaItems.begin(); it != bungaItems.end(); ) {
        (*it)->Update(envDt);
        if ((*it)->pos.y > screenHeight + (*it)->radius) it = bungaItems.erase(it);
        else ++it;
    }

    // Update Tali Tangsi
    for (auto it = tangsiItems.begin(); it != tangsiItems.end(); ) {
        (*it)->Update(envDt);
        if ((*it)->pos.y > screenHeight + (*it)->radius) it = tangsiItems.erase(it);
        else ++it;
    }

    // Update Ais Kacang
    for (auto it = aisKacangItems.begin(); it != aisKacangItems.end(); ) {
        (*it)->Update(envDt);
        if ((*it)->pos.y > screenHeight + (*it)->radius) it = aisKacangItems.erase(it);
        else ++it;
    }

    // Update Coin
    for (auto it = coinItems.begin(); it != coinItems.end(); ) {
        (*it)->Update(envDt);
        if ((*it)->pos.y > screenHeight + (*it)->radius) it = coinItems.erase(it);
        else ++it;
    }
}

void ItemSpawner::Draw() {
    for (const auto& item : bungaItems) item->Draw();
    for (const auto& item : tangsiItems) item->Draw();
    for (const auto& item : coinItems) item->Draw();
    for (const auto& item : aisKacangItems) item->Draw();
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

int ItemSpawner::CheckAisKacangCollisions(Vector2 wauPos, float wauRadius, Vector2 kidHitboxPos, float kidRadius) {
    bool collected = false;
    for (auto it = aisKacangItems.begin(); it != aisKacangItems.end(); ) {
        if ((*it)->CheckCollision(wauPos, wauRadius) || (*it)->CheckCollision(kidHitboxPos, kidRadius)) {
            slowMoTimer = 5.0f; // Start 5s slow-mo window
            collected = true;
            it = aisKacangItems.erase(it);
        } else {
            ++it;
        }
    }
    return collected;
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

bool ItemSpawner::IsBoostActive() const { return boostTimer > 0.0f; }
bool ItemSpawner::IsSlowMoActive() const { return slowMoTimer > 0.0f; }

void ItemSpawner::Reset() {
    bungaItems.clear();
    tangsiItems.clear();
    aisKacangItems.clear();
    coinItems.clear();

    // Randomize initial timers to stagger their first appearance organically
    bungaSpawnTimer = (float)GetRandomValue(0, 5);                 
    tangsiSpawnTimer = (float)GetRandomValue(5, 10);                
    aisKacangSpawnTimer = (float)GetRandomValue(10, 20);          
    
    // Coins spawn every 2.5s. A slight random start prevents predictable early drops
    coinSpawnTimer = (float)GetRandomValue(0, 15) / 10.0f;

    boostTimer = 0.0f;
    slowMoTimer = 0.0f;
}

void ItemSpawner::Unload() {
    UnloadTexture(texBungaRaya);
    UnloadTexture(texTangsi);
    UnloadTexture(texAisKacang);
    UnloadTexture(texCoin);
}