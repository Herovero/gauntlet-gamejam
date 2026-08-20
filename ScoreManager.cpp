#include "ScoreManager.hpp"

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
    // Draw semi transparent panel
    DrawRectangle(10, 10, 260, 85, Fade(BLACK, 0.5f));

    // Draw current altitude at the top left
    const char* text = TextFormat("Altitude: %.0f m", currentAltitude);
    DrawText(text, 20, 20, 30, BLUE);

    // Draw remaining string
    DrawText(TextFormat("Tali Tangsi: %d", stringCharges), 20, 60, 20, GOLD);
}

void ScoreManager::DrawGameOver(int screenWidth, int screenHeight) {
    // Draw semi transparent panel
    int panelWidth = 500;
    int panelHeight = 300;
    int panelX = screenWidth / 2 - panelWidth / 2;
    int panelY = screenHeight / 2 - 140;
    
    DrawRectangle(panelX, panelY, panelWidth, panelHeight, Fade(BLACK, 0.7f));

    const char* title = "GAME OVER";
    const char* currentText = TextFormat("Altitude Reached: %.0f m", currentAltitude);
    const char* highText = TextFormat("Highest Altitude: %.0f m", highestAltitude);
    const char* restartText = "Press SPACE to Restart";

    // Use MeasureText to center everything based on font size
    DrawText(title, screenWidth / 2 - MeasureText(title, 50) / 2, screenHeight / 2 - 100, 50, RED);
    DrawText(currentText, screenWidth / 2 - MeasureText(currentText, 30) / 2, screenHeight / 2 - 20, 30, GREEN);
    DrawText(highText, screenWidth / 2 - MeasureText(highText, 25) / 2, screenHeight / 2 + 25, 25, GOLD);
    DrawText(restartText, screenWidth / 2 - MeasureText(restartText, 20) / 2, screenHeight / 2 + 80, 20, WHITE);
}

void ScoreManager::Reset() {
    currentAltitude = 0.0f;
    stringCharges = 3;
}