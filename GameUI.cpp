#include "GameUI.hpp"

bool GameUI::IsShopButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos) {
    Rectangle shopBtn = { (float)screenWidth / 2.0f - 100.0f, (float)screenHeight / 2.0f + 60.0f, 200.0f, 50.0f };
    return CheckCollisionPointRec(virtualMousePos, shopBtn);
}

void GameUI::DrawMainMenu(int screenWidth, int screenHeight, Vector2 virtualMousePos) {
    // Semi transparent panel 
    int panelWidth = 650;
    int panelHeight = 200;
    DrawRectangle(screenWidth / 2 - panelWidth / 2, screenHeight / 2 - 130, panelWidth, panelHeight, Fade(BLACK, 0.6f));

    DrawText("WAU BULAN RISING", screenWidth / 2 - MeasureText("WAU BULAN RISING", 60) / 2, screenHeight / 2 - 100, 60, DARKBLUE);
    DrawText("Press SPACE to Take Off!", screenWidth / 2 - MeasureText("Press SPACE to Take Off!", 30) / 2, screenHeight / 2, 30, YELLOW);

    // Draw the Shop Button
    Rectangle shopBtn = { (float)screenWidth / 2.0f - 100.0f, (float)screenHeight / 2.0f + 60.0f, 200.0f, 50.0f };
    Color btnColor = IsShopButtonClicked(screenWidth, screenHeight, virtualMousePos) ? LIGHTGRAY : DARKGRAY;
    DrawRectangleRec(shopBtn, btnColor);
    DrawText("OPEN SHOP", screenWidth / 2 - MeasureText("OPEN SHOP", 20) / 2, screenHeight / 2 + 75, 20, WHITE);
}

void GameUI::DrawVictoryScreen(int screenWidth, int screenHeight) {
    int panelWidth = 550;
    int panelHeight = 250;
    int panelX = screenWidth / 2 - panelWidth / 2;
    int panelY = screenHeight / 2 - 120;
    
    DrawRectangle(panelX, panelY, panelWidth, panelHeight, Fade(BLACK, 0.7f));
    DrawText("MERDEKA!", screenWidth / 2 - MeasureText("MERDEKA!", 60) / 2, screenHeight / 2 - 90, 60, GOLD);
    DrawText("You Reached the Top!", screenWidth / 2 - MeasureText("You Reached the Top!", 30) / 2, screenHeight / 2 - 10, 30, RAYWHITE);
    DrawText("Press SPACE to Play Again", screenWidth / 2 - MeasureText("Press SPACE to Play Again", 20) / 2, screenHeight / 2 + 70, 20, YELLOW);
}