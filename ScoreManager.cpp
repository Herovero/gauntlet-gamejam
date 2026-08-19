#include "ScoreManager.hpp"

ScoreManager::ScoreManager() {
    currentAltitude = 0.0f;
    highestAltitude = 0.0f;
    climbRate = 5.0f;
}

void ScoreManager::Update(float dt, bool isDetached) {
    // Only increase score if the kid is still attached to the kite
    if (!isDetached) {
        currentAltitude += climbRate * dt;
        
        // Continuously update the high score if we surpass it
        if (currentAltitude > highestAltitude) {
            highestAltitude = currentAltitude;
        }
    }
}

void ScoreManager::Draw() {
    // Draw current altitude at the top left during gameplay
    const char* text = TextFormat("Altitude: %.0f m", currentAltitude);
    DrawText(text, 20, 20, 30, DARKGRAY);
}

void ScoreManager::DrawGameOver(int screenWidth, int screenHeight) {
    const char* title = "GAME OVER";
    const char* currentText = TextFormat("Altitude Reached: %.0f m", currentAltitude);
    const char* highText = TextFormat("Highest Altitude: %.0f m", highestAltitude);
    const char* restartText = "Press SPACE to Restart";

    // Use MeasureText to center everything based on font size
    DrawText(title, screenWidth / 2 - MeasureText(title, 50) / 2, screenHeight / 2 - 100, 50, RED);
    DrawText(currentText, screenWidth / 2 - MeasureText(currentText, 30) / 2, screenHeight / 2 - 20, 30, BLACK);
    DrawText(highText, screenWidth / 2 - MeasureText(highText, 25) / 2, screenHeight / 2 + 25, 25, DARKGRAY);
    DrawText(restartText, screenWidth / 2 - MeasureText(restartText, 20) / 2, screenHeight / 2 + 80, 20, GRAY);
}

void ScoreManager::Reset() {
    // Reset current score
    currentAltitude = 0.0f;
}