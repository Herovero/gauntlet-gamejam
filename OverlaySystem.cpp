#include "OverlaySystem.hpp"
#include "GameUI.hpp"
#include "GameManager.hpp"

GameState OverlaySystem::Update(GameState currentState, int screenWidth, int screenHeight, Vector2 virtualMousePos,
                                AudioManager& audio, WauBulan& wau, SwingingKid& kid, Background& bg,
                                ObstacleSpawner& spawner, ItemSpawner& itemSpawner, ScoreManager& scoreManager,
                                CoinManager& coinManager, WindForce& wind, KipasSatay& kipas, ShopManager& shopManager,
                                float& menuIntroTimer, bool& introFinished, float currentStringLength) {
    
    bool resumeHovered = GameUI::IsRestartButtonClicked(screenWidth, screenHeight, virtualMousePos);
    bool menuHovered = GameUI::IsMainMenuButtonClicked(screenWidth, screenHeight, virtualMousePos);

    // Allow unpausing with the keyboard
    if (currentState == PAUSED && (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P))) {
        return PLAYING;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (resumeHovered) {
            if (currentState == VICTORY) {
                audio.PlayGameBGM();
                GameManager::ResetGame(screenWidth, screenHeight, wau, kid, bg, spawner, itemSpawner, scoreManager, coinManager, wind, kipas);
                
                KiteProfile equipped = shopManager.GetEquippedKite();
                wau.speed = equipped.speed;
                wau.radius = 25.0f * equipped.sizeMultiplier;
                wau.ChangeTexture(equipped.texturePath.c_str());
                wau.pos.y = screenHeight - 600.0f; 
                kid.pos.y = wau.pos.y + currentStringLength;
            }
            return PLAYING; // Resumes immediately if paused, or starts a fresh run if victorious
        }
        else if (menuHovered) {
            audio.PlayMenuBGM();
            GameManager::ResetGame(screenWidth, screenHeight, wau, kid, bg, spawner, itemSpawner, scoreManager, coinManager, wind, kipas);
            
            menuIntroTimer = 0.0f;
            introFinished = false;
            wau.Reset((float)screenWidth / 2.0f - 250.0f, (float)screenHeight + 200.0f); 
            kid.pos = { wau.pos.x, wau.pos.y + 550.0f };
            return MENU;
        }
    }
    
    return currentState;
}