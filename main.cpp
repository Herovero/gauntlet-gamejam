#include "raylib.h"
#include "GameManager.hpp"
#include "AudioManager.hpp"
#include "WauBulan.hpp"
#include "GameUI.hpp"
#include "VirtualCanvas.hpp"
#include "SwingingKid.hpp"
#include "Background.hpp"
#include "ObstacleSpawner.hpp"
#include "CollisionManager.hpp"
#include "ScoreManager.hpp"
#include "CoinManager.hpp"
#include "ShopManager.hpp"
#include "ItemSpawner.hpp"
#include "WindForce.hpp"
#include "KipasSatay.hpp"
#include <cmath>

enum GameState {
    MENU,
    SHOP,
    PLAYING,
    GAMEOVER,
    VICTORY
};

int main() {
    // Screen Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;

    // Run the window and show title
    InitWindow(screenWidth, screenHeight, "Wau Bulan Rising");

    // Enable resizing and configure the window
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // Initialize audio to load mp3
    InitAudioDevice();
    AudioManager audio;
    audio.PlayMenuBGM();

    // Framerate per second
    SetTargetFPS(60); 

    Background bg("assets/background2.png", screenWidth, screenHeight, 30.0f);
    Background menuBg("assets/skybackground.png", screenWidth, screenHeight, 150.0f);

    Texture2D gameTitle = LoadTexture("assets/game_title.png");

    WauBulan wau(screenWidth / 2.0f, screenHeight - 600.0f, "assets/waubulan.png");
    SwingingKid kid(wau.pos, "assets/kid_swinging.png", "assets/kid_falling.png", "assets/kid_swinging.png");
    VirtualCanvas canvas(screenWidth, screenHeight);
    ObstacleSpawner spawner(screenWidth, screenHeight);
    ItemSpawner itemSpawner(screenWidth, screenHeight);
    ScoreManager scoreManager;
    CoinManager coinManager;
    ShopManager shopManager;
    WindForce wind;
    KipasSatay kipas;

    const float NORMAL_BG_SPEED = 30.0f;
    const float BOOST_BG_SPEED = 150.0f;

    // Initialize State Machine
    GameState gameState = MENU;

    float introTimer = 0.0f;
    bool introFinished = false;

    // Set up the menu positions
    wau.pos = { (float)screenWidth / 2.0f - 250.0f, (float)screenHeight + 200.0f };
    kid.pos = { wau.pos.x, wau.pos.y + 550.0f };
    kid.isOnGround = false;

    // Main Game Loop
    // WindowShouldClose() returns true if pressing escape or close buton
    while (!WindowShouldClose()) { 
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();
        
        float dt = GetFrameTime();

        audio.Update();

        canvas.UpdateScaling();
        Vector2 virtualMousePos = canvas.GetVirtualMousePosition();

        if (gameState == MENU) {
            // Infinite scrolling logic handled by your new class!
            menuBg.UpdateInfinite(dt);

            // Intro Rising Animation
            if (!introFinished) {
                introTimer += dt;
                float duration = 3.0f; 
                float startY = screenHeight + 200.0f;
                float endY = screenHeight - 600.0f;

                if (introTimer < duration) {
                    float t = introTimer / duration;
                    float easeOut = 1.0f - (1.0f - t) * (1.0f - t); 
                    wau.pos.y = startY + (endY - startY) * easeOut;
                    kid.pos.y = wau.pos.y + 550.0f;
                } else {
                    wau.pos.y = endY;
                    kid.pos.y = wau.pos.y + 550.0f;
                    introFinished = true;
                }
            } 
            // UI Interactions (Only after rising finishes)
            else {
                wau.pos.y = (screenHeight - 600.0f) + std::sin(GetTime() * 3.0f) * 10.0f;
                kid.pos.y = wau.pos.y + 550.0f;

                bool startHovered = GameUI::IsStartButtonClicked(screenWidth, screenHeight, virtualMousePos);
                bool shopHovered = GameUI::IsShopButtonClicked(screenWidth, screenHeight, virtualMousePos);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (startHovered) {
                        gameState = PLAYING;
                        kid.isOnGround = false;

                        audio.PlayGameBGM();
                        
                        KiteProfile equipped = shopManager.GetEquippedKite();
                        wau.speed = equipped.speed;
                        wau.radius = 25.0f * equipped.sizeMultiplier;
                        wau.ChangeTexture(equipped.texturePath.c_str());
                        
                        // Drop them down to the correct gameplay starting position
                        wau.pos.x = (float)screenWidth / 2.0f;
                        wau.pos.y = screenHeight - 600.0f; 
                        kid.pos.x = wau.pos.x;
                        kid.pos.y = wau.pos.y + 550.0f;
                    } 
                    else if (shopHovered) {
                        gameState = SHOP;
                    }
                }
            }
        }
        else if (gameState == SHOP) {
            bool returnToMenu = false;

            int coinsBefore = coinManager.totalCoins;

            shopManager.Update(coinManager.totalCoins, returnToMenu, dt, screenWidth, virtualMousePos);

            // If your balance went down, you successfully bought a kite
            if (coinManager.totalCoins < coinsBefore) {
                audio.PlayPurchase();
            }

            if (returnToMenu) {
                gameState = MENU;
            }
        }
        else if (gameState == PLAYING) {
            // Apply boost speed if active
            bg.scrollSpeed = itemSpawner.IsBoostActive() ? BOOST_BG_SPEED : NORMAL_BG_SPEED;
            bg.Update(dt);

            if (!kid.isDetached) wau.Update(dt, screenWidth, screenHeight);
            else wau.pos.y += 400.0f * dt;
            
            bool windWasActive = wind.IsActive();
            wind.Update(dt);
            if (!windWasActive && wind.IsActive()) {
                audio.PlayWind(); 
            }

            kipas.Update(dt, screenWidth, wind.IsActive(), wind.IsWindFromLeft());
            kid.Update(dt, wau.pos, wind.GetForce());
            scoreManager.Update(dt, kid.isDetached, itemSpawner.IsBoostActive());
            spawner.Update(dt, scoreManager.currentAltitude, bg.scrollSpeed, audio);
            itemSpawner.Update(dt);

            CollisionManager::HandleItemCollections(wau, kid, itemSpawner, scoreManager, coinManager, audio);

            // Check obstacle collisions
            if (!kid.isDetached && wau.invincibleTimer <= 0.0f && CollisionManager::CheckPlayerCollisions(wau, kid, spawner)) {
                audio.PlayHit();
                kid.Detach(wau.pos.x);
            }

            // Check for string recovery
            bool wasDetached = kid.isDetached;

            kid.TryReattach(virtualMousePos, wau.pos.x, wau.pos.y, wau.radius, wau.invincibleTimer, scoreManager.stringCharges);

            // If they were detached but aren't anymore, the catch was successful!
            if (wasDetached && !kid.isDetached) {
                audio.PlaySnap();
            }

            // Trigger the game over screen when the kid drops out of view
            if (kid.isDetached && (kid.pos.y - kid.radius) > (float)screenHeight) {
                gameState = GAMEOVER;

                audio.StopBGM();
            }

            if (bg.IsAtTop()) {
                gameState = VICTORY;

                audio.StopBGM();
            }
        }
        else if (gameState == GAMEOVER || gameState == VICTORY) {
            bool restartHovered = GameUI::IsRestartButtonClicked(screenWidth, screenHeight, virtualMousePos);
            bool menuHovered = GameUI::IsMainMenuButtonClicked(screenWidth, screenHeight, virtualMousePos);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (restartHovered) {
                    gameState = PLAYING;
                    audio.PlayGameBGM();
                    
                    GameManager::ResetGame(screenWidth, screenHeight, wau, kid, bg, spawner, itemSpawner, 
                        scoreManager, coinManager, wind, kipas);
                        
                    // Re-apply equipped kite stats directly for a fast restart
                    KiteProfile equipped = shopManager.GetEquippedKite();
                    wau.speed = equipped.speed;
                    wau.radius = 25.0f * equipped.sizeMultiplier;
                    wau.ChangeTexture(equipped.texturePath.c_str());
                    
                    wau.pos.y = screenHeight - 600.0f; 
                    kid.pos.y = wau.pos.y + 550.0f;
                    kid.isOnGround = false;
                }
                else if (menuHovered) {
                    gameState = MENU;
                    audio.PlayMenuBGM();
                    
                    GameManager::ResetGame(screenWidth, screenHeight, wau, kid, bg, spawner, itemSpawner, 
                        scoreManager, coinManager, wind, kipas);
                    
                    // Reset Intro Animation State for the menu
                    introTimer = 0.0f;
                    introFinished = false;
                    wau.Reset((float)screenWidth / 2.0f - 250.0f, (float)screenHeight + 200.0f); 
                    kid.pos = { wau.pos.x, wau.pos.y + 550.0f };
                }
            }
        }

        // Drawing logic
        canvas.BeginMode();
            if (gameState == SHOP) {
                shopManager.Draw(screenWidth, screenHeight, coinManager.totalCoins, virtualMousePos);
            }
            else if (gameState == MENU) {
                menuBg.DrawInfinite(); // Uses your new looping method!
                
                wau.Draw();
                kid.Draw(wau.pos);
                GameUI::DrawMainMenu(screenWidth, screenHeight, virtualMousePos, introFinished, gameTitle);
            }
            else {
                ClearBackground(SKYBLUE);
                bg.Draw();

                if (gameState == PLAYING) {
                    wau.Draw();
                    wind.Draw(screenWidth);
                    kipas.Draw(screenHeight, wind.IsActive(), wind.IsWindFromLeft());
                    kid.Draw(wau.pos, wau.invincibleTimer);
                    spawner.Draw();
                    itemSpawner.Draw();
                    scoreManager.Draw();
                    coinManager.Draw(screenWidth);
                } 
                else if (gameState == GAMEOVER) {
                    scoreManager.DrawGameOver(screenWidth, screenHeight);

                    GameUI::DrawEndGameButtons(screenWidth, screenHeight, virtualMousePos);
                }
                else if (gameState == VICTORY) {
                    wau.Draw();
                    kid.Draw(wau.pos);
                    spawner.Draw();
                    GameUI::DrawVictoryScreen(screenWidth, screenHeight);
                    GameUI::DrawEndGameButtons(screenWidth, screenHeight, virtualMousePos);
                }
            }

        canvas.EndModeAndDraw();
    }

    wau.Unload();
    kid.Unload();
    bg.Unload();
    menuBg.Unload();
    spawner.Unload();
    itemSpawner.Unload();
    kipas.Unload();
    shopManager.Unload();

    UnloadTexture(gameTitle);
    audio.Unload();
    CloseAudioDevice();
    CloseWindow(); 

    return 0;
}

