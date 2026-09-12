#pragma once
#include "raylib.h"
#include <string>

class CoinManager {
public:
    int totalCoins;
    Texture2D uiCoinIcon;

    CoinManager();
    void AddCoins(int amount);
    void Draw(int screenWidth);
    void DrawGameOver(int screenWidth, int screenHeight);
    void Reset();
    void Unload();
};