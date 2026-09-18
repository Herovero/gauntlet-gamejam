#pragma once
#include "raylib.h"
#include <vector>
#include <string>

// This struct holds the Hungry Shark style traits
struct KiteProfile {
    std::string name;
    std::string texturePath;
    float speed;             // Trait 1: Movement speed
    float sizeMultiplier;    // Trait 2: Hitbox size (smaller is easier to dodge)
    int price;
    bool isUnlocked;
};

class ShopManager {
public:
    std::vector<KiteProfile> kites;
    int currentIndex;
    int equippedIndex;

    ShopManager();
    void Update(int& playerCoins, bool& returnToMenu);
    void Draw(int screenWidth, int screenHeight, int playerCoins);
    KiteProfile GetEquippedKite();
};