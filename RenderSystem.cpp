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