#include "CoinManager.hpp"

CoinManager::CoinManager() {
    totalCoins = 0;
    
    // We can reuse the same coin image for the UI
    uiCoinIcon = LoadTexture("assets/coin.png");
    
    // Keep it crisp if it is pixel art
    if (uiCoinIcon.id > 0) {
        SetTextureFilter(uiCoinIcon, TEXTURE_FILTER_POINT);
    }
}

void CoinManager::AddCoins(int amount) {
    totalCoins += amount;
}

void CoinManager::Draw(int screenWidth) {
    std::string coinText = "x " + std::to_string(totalCoins);
    
    // Position it in the top right corner
    float xPos = (float)screenWidth - 150.0f;
    float yPos = 30.0f;

    if (uiCoinIcon.id > 0) {
        // Draw the coin icon next to the text
        float renderSize = 70.0f;
        Rectangle source = { 0.0f, 0.0f, (float)uiCoinIcon.width, (float)uiCoinIcon.height };
        Rectangle dest = { xPos, yPos, renderSize, renderSize };
        DrawTexturePro(uiCoinIcon, source, dest, { 0.0f, 0.0f }, 0.0f, WHITE);
    } else {
        DrawCircleV({ xPos + 15.0f, yPos + 15.0f }, 15.0f, MAGENTA);
    }

    // Draw the actual coin amount
    DrawText(coinText.c_str(), (int)xPos + 90, (int)yPos + 20, 24, GOLD);
}

void CoinManager::DrawGameOver(int screenWidth, int screenHeight) {
    std::string text = "Total Coins: " + std::to_string(totalCoins);
    DrawText(text.c_str(), screenWidth / 2 - MeasureText(text.c_str(), 30) / 2, screenHeight / 2 + 30, 30, GOLD);
}

void CoinManager::Reset() {
    totalCoins = 0;
}

void CoinManager::Unload() {
    UnloadTexture(uiCoinIcon);
}