#include "RenderSystem.hpp"
#include "GameUI.hpp"

void RenderSystem::Draw(GameState gameState, int screenWidth, int screenHeight, Vector2 virtualMousePos,
                        bool introFinished, Texture2D gameTitle, Texture2D frozenBg,
                        Background& bg, Background& menuBg, WauBulan& wau, SwingingKid& kid,
                        ShopManager& shopManager, CoinManager& coinManager, WindForce& wind,
                        KipasSatay& kipas, ObstacleSpawner& spawner, ItemSpawner& itemSpawner,
                        ParticleManager& particleManager, ScoreManager& scoreManager) {
    
    if (gameState == SHOP) {
        shopManager.Draw(screenWidth, screenHeight, coinManager.totalCoins, virtualMousePos);
    }
    else if (gameState == MENU) {
        menuBg.DrawInfinite(); 
        wau.Draw();
        kid.Draw(wau.pos);
        GameUI::DrawMainMenu(screenWidth, screenHeight, virtualMousePos, introFinished, gameTitle);

        if (introFinished) {
    #if defined(__ANDROID__)
            // Updated width and X position to match Start/Shop buttons
            Rectangle toggleBtn = { (float)screenWidth / 2.0f + 120.0f, (float)screenHeight / 2.0f + 60.0f, 260.0f, 46.0f };
            
            bool isHovered = CheckCollisionPointRec(virtualMousePos, toggleBtn);
            Color btnBg = isHovered ? GetColor(0x8D6E63FF) : GetColor(0x5D4037FF);
            DrawRectangleRounded(toggleBtn, 0.2f, 4, btnBg);
            DrawRectangleRoundedLines(toggleBtn, 0.2f, 4, isHovered ? GOLD : GetColor(0xD4AF37FF));
            
            const char* btnText = wau.isTiltEnabled ? "CONTROL: TILT" : "CONTROL: JOYSTICK";
            DrawText(btnText, screenWidth / 2 + 250 - MeasureText(btnText, 20) / 2, screenHeight / 2 + 73, 20, RAYWHITE);
    #endif
        }
    }
    else {
        ClearBackground(SKYBLUE);
        bg.Draw();

        if (gameState == PLAYING || gameState == WAITING_TO_REWIND) {
            wau.Draw();
            wind.Draw(screenWidth);
            kipas.Draw(screenHeight, wind.IsActive(), wind.IsWindFromLeft());
            kid.Draw(wau.pos, wau.invincibleTimer);
            spawner.Draw();
            itemSpawner.Draw();
            particleManager.Draw();
            scoreManager.Draw();
            coinManager.Draw(screenWidth);

            if (itemSpawner.IsSlowMoActive()) {
                Rectangle source = { 0.0f, 0.0f, (float)frozenBg.width, (float)frozenBg.height };
                Rectangle dest = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
                DrawTexturePro(frozenBg, source, dest, { 0.0f, 0.0f }, 0.0f, Fade(WHITE, 0.3f));
                DrawRectangle(0, 0, screenWidth, screenHeight, Fade(SKYBLUE, 0.25f));
                DrawRectangleLinesEx({0, 0, (float)screenWidth, (float)screenHeight}, 10.0f, Fade(WHITE, 0.4f));
            }
        }
        else if (gameState == REWINDING) {
            // Characters and obstacles are hidden while reversing
        }
        else if (gameState == FALLING_DOWN || gameState == RISING) {
            wau.Draw();
            kid.Draw(wau.pos, wau.invincibleTimer);
        }
        else if (gameState == PAUSED) {
            wau.Draw();
            wind.Draw(screenWidth);
            kipas.Draw(screenHeight, wind.IsActive(), wind.IsWindFromLeft());
            kid.Draw(wau.pos, wau.invincibleTimer);
            spawner.Draw();
            itemSpawner.Draw();
            particleManager.Draw();
            scoreManager.Draw();
            coinManager.Draw(screenWidth);

            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
            DrawText("PAUSED", screenWidth / 2 - MeasureText("PAUSED", 60) / 2, screenHeight / 2 - 150, 60, WHITE);
            GameUI::DrawEndGameButtons(screenWidth, screenHeight, virtualMousePos, true);
        }
        else if (gameState == VICTORY) {
            wau.Draw();
            kid.Draw(wau.pos);
            spawner.Draw();
            GameUI::DrawVictoryScreen(screenWidth, screenHeight);
            GameUI::DrawEndGameButtons(screenWidth, screenHeight, virtualMousePos, false);
        }
    }
}