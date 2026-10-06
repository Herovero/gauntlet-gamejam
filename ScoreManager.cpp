#include "ScoreManager.hpp"
#include "GameUI.hpp"

ScoreManager::ScoreManager() {
    currentAltitude = 0.0f;
    highestAltitude = 0.0f;
    baseClimbRate = 5.0f;
    boostClimbRate = 25.0f;
    stringCharges = 3;
}

void ScoreManager::Update(float dt, bool isDetached, bool isBoosted) {
    // Only increase score if the kid is still attached to the kite
    if (!isDetached) {
        float activeRate = isBoosted ? boostClimbRate : baseClimbRate;
        currentAltitude += activeRate * dt;
        
        // Continuously update the high score if we surpass it
        if (currentAltitude > highestAltitude) {
            highestAltitude = currentAltitude;
        }
    }
}

void ScoreManager::Draw() {
    // 1. Draw a smaller traditional frame for the HUD
    Rectangle hudPanel = { 10.0f, 10.0f, 260.0f, 100.0f };
    GameUI::DrawTraditionalFrame(hudPanel);

    // 2. Use dark brown colors instead of neon blue/yellow to contrast with the rice paper
    Color textDark = GetColor(0x3E2723FF); 
    
    DrawText(TextFormat("Altitude: %d m", (int)currentAltitude), 30, 25, 24, textDark);
    DrawText(TextFormat("Tali Tangsi: %d", stringCharges), 30, 65, 24, textDark);
}

void ScoreManager::DrawGameOver(int screenWidth, int screenHeight) {
    // 1. Create a panel large enough to hold the text AND the buttons
    int panelWidth = 540;
    int panelHeight = 320;
    Rectangle gameOverPanel = {
        (float)(screenWidth / 2 - panelWidth / 2),
        (float)(screenHeight / 2 - 160),
        (float)panelWidth,
        (float)panelHeight
    };

    GameUI::DrawTraditionalFrame(gameOverPanel);

    // 2. Adjust Y-coordinates to push the text up, preventing overlap with the buttons
    DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 50) / 2, screenHeight / 2 - 120, 50, GetColor(0x8B0000FF)); // Dark Red
    
    Color textDark = GetColor(0x3E2723FF);
    DrawText(TextFormat("Altitude Reached: %d m", (int)currentAltitude), screenWidth / 2 - MeasureText(TextFormat("Altitude Reached: %d m", (int)currentAltitude), 28) / 2, screenHeight / 2 - 50, 28, textDark);
    DrawText(TextFormat("Highest Altitude: %d m", (int)highestAltitude), screenWidth / 2 - MeasureText(TextFormat("Highest Altitude: %d m", (int)highestAltitude), 22) / 2, screenHeight / 2 - 10, 22, textDark);
    
    // Note: The "Press SPACE to Restart" text has been intentionally removed here 
    // since you are now using the clickable buttons from GameUI!
}

void ScoreManager::Reset() {
    currentAltitude = 0.0f;
    stringCharges = 3;
}