#include "raylib.h"
#include "GameManager.hpp"
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
    Music bgm = LoadMusicStream("assets/bgm.mp3");
    SetMusicVolume(bgm, 1.0f);

    Music menuBgm = LoadMusicStream("assets/menu_bgm.mp3");
    SetMusicVolume(menuBgm, 1.0f);

    Sound sfxHit = LoadSound("assets/hit.wav");
    SetSoundVolume(sfxHit, 0.8f);

    Sound sfxItem = LoadSound("assets/pickup.wav");
    SetSoundVolume(sfxItem, 0.9f);

    // Framerate per second
    SetTargetFPS(60); 

    Background bg("assets/background2.png", screenWidth, screenHeight, 30.0f);
    Background menuBg("assets/skybackground.png", screenWidth, screenHeight, 150.0f);

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
    wau.pos = { (float)screenWidth / 2.0f, (float)screenHeight + 200.0f };
    kid.pos = { wau.pos.x, wau.pos.y + 550.0f };
    kid.isOnGround = false;

    // Start the menu music
    PlayMusicStream(menuBgm);

    // Main Game Loop
    // WindowShouldClose() returns true if pressing escape or close buton
    while (!WindowShouldClose()) { 
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();
        
        float dt = GetFrameTime();

        UpdateMusicStream(bgm);
        UpdateMusicStream(menuBgm);

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

                        StopMusicStream(menuBgm);
                        PlayMusicStream(bgm);
                        
                        KiteProfile equipped = shopManager.GetEquippedKite();
                        wau.speed = equipped.speed;
                        wau.radius = 25.0f * equipped.sizeMultiplier;
                        
                        // Drop them down to the correct gameplay starting position
                        wau.pos.y = screenHeight - 600.0f; 
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
            // Pass in totalCoins from your CoinManager
            shopManager.Update(coinManager.totalCoins, returnToMenu, dt, screenWidth); 
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

            wind.Update(dt);
            kipas.Update(dt, screenWidth, wind.IsActive(), wind.IsWindFromLeft());
            kid.Update(dt, wau.pos, wind.GetForce());
            scoreManager.Update(dt, kid.isDetached, itemSpawner.IsBoostActive());
            spawner.Update(dt, scoreManager.currentAltitude, bg.scrollSpeed);
            itemSpawner.Update(dt);

            CollisionManager::HandleItemCollections(wau, kid, itemSpawner, scoreManager, coinManager, sfxItem);

            // Check obstacle collisions
            if (!kid.isDetached && wau.invincibleTimer <= 0.0f && CollisionManager::CheckPlayerCollisions(wau, kid, spawner)) {
                PlaySound(sfxHit);
                kid.Detach(wau.pos.x);
            }

            // Check for string recovery
            kid.TryReattach(virtualMousePos, wau.pos.x, wau.pos.y, wau.radius, wau.invincibleTimer, scoreManager.stringCharges);

            // Trigger the game over screen when the kid drops out of view
            if (kid.isDetached && (kid.pos.y - kid.radius) > (float)screenHeight) {
                gameState = GAMEOVER;

                StopMusicStream(bgm);
            }

            if (bg.IsAtTop()) {
                gameState = VICTORY;

                StopMusicStream(bgm);
            }
        }
        else if (gameState == GAMEOVER || gameState == VICTORY) {
            if (IsKeyPressed(KEY_SPACE)) {
                gameState = MENU;

                PlayMusicStream(menuBgm);

                GameManager::ResetGame(screenWidth, screenHeight, wau, kid, bg, spawner, itemSpawner, 
                    scoreManager, coinManager, wind, kipas);
                
                // Reset Intro Animation State
                introTimer = 0.0f;
                introFinished = false;
                wau.Reset((float)screenWidth / 2.0f, (float)screenHeight + 200.0f);
                kid.pos = { wau.pos.x, wau.pos.y + 550.0f };
            }
        }

        // Drawing logic
        canvas.BeginMode();
            if (gameState == SHOP) {
                shopManager.Draw(screenWidth, screenHeight, coinManager.totalCoins);
            }
            else if (gameState == MENU) {
                menuBg.DrawInfinite(); // Uses your new looping method!
                
                wau.Draw();
                kid.Draw(wau.pos);
                GameUI::DrawMainMenu(screenWidth, screenHeight, virtualMousePos, introFinished);
            }
            else {
                ClearBackground(SKYBLUE);
                bg.Draw();

                if (gameState == PLAYING) {
                    wau.Draw();
                    wind.Draw(screenWidth);
                    kipas.Draw(screenHeight, wind.IsActive(), wind.IsWindFromLeft());
                    kid.Draw(wau.pos);
                    spawner.Draw();
                    itemSpawner.Draw();
                    scoreManager.Draw();
                    coinManager.Draw(screenWidth);
                } 
                else if (gameState == GAMEOVER) {
                    scoreManager.DrawGameOver(screenWidth, screenHeight);
                }
                else if (gameState == VICTORY) {
                    wau.Draw();
                    kid.Draw(wau.pos);
                    spawner.Draw();
                    GameUI::DrawVictoryScreen(screenWidth, screenHeight);
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

    UnloadSound(sfxHit);
    UnloadSound(sfxItem);
    UnloadMusicStream(menuBgm);
    UnloadMusicStream(bgm);
    CloseAudioDevice();
    CloseWindow(); 

    return 0;
}

