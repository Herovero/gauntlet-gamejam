#pragma once
#include "raylib.h"
#include <vector>
#include <string>

struct KiteProfile {
    std::string name;
    std::string texturePath;
    Texture2D texture;
    float speed;            
    float sizeMultiplier;   
    int price;
    bool isUnlocked;
};

class ShopManager {
public:
    std::vector<KiteProfile> kites;
    int currentIndex;
    int equippedIndex;

    Texture2D bgTexture;

    float currentX;
    float targetX;

    ShopManager();
    void Update(int& playerCoins, bool& returnToMenu, float dt, int screenWidth);
    void Draw(int screenWidth, int screenHeight, int playerCoins);
    KiteProfile GetEquippedKite();

    void Unload();
};