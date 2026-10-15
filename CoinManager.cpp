#include "CoinManager.hpp"

CoinManager::CoinManager() {
    totalCoins = 0;
    
    uiCoinIcon = LoadTexture("assets/coin.png");
    
    GenTextureMipmaps(&uiCoinIcon);
    SetTextureFilter(uiCoinIcon, TEXTURE_FILTER_TRILINEAR);
}

void CoinManager::AddCoins(int amount) {
    totalCoins += amount;
}

void CoinManager::Draw() {
    std::string coinText = "x " + std::to_string(totalCoins);
    
    // Positioned to align underneath the Tali Tangsi icon inside the HUD
    float xPos = 30.0f; 
    float yPos = 100.0f; 

    if (uiCoinIcon.id > 0) {
        float renderSize = 40.0f; // Scaled down to match the HUD text size
        Rectangle source = { 0.0f, 0.0f, (float)uiCoinIcon.width, (float)uiCoinIcon.height };
        Rectangle dest = { xPos, yPos, renderSize, renderSize };
        DrawTexturePro(uiCoinIcon, source, dest, { 0.0f, 0.0f }, 0.0f, WHITE);
    }

    // Draw the coin amount text in the same brown color as the Altitude text
    DrawText(coinText.c_str(), (int)xPos + 50, (int)yPos + 8, 24, GetColor(0x3E2723FF));
}

void CoinManager::DrawGameOver(int screenWidth, int screenHeight) {
    std::string text = "Total Coins: " + std::to_string(totalCoins);
    DrawText(text.c_str(), screenWidth / 2 - MeasureText(text.c_str(), 30) / 2, screenHeight / 2 + 30, 30, GOLD);
}

void CoinManager::Reset() {}

void CoinManager::Unload() {
    UnloadTexture(uiCoinIcon);
}