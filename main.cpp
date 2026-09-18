#include "raylib.h"
#include "WauBulan.hpp"
#include "GameUI.hpp"
#include "VirtualCanvas.hpp"
#include "SwingingKid.hpp"
#include "Obstacle.hpp"
#include "Background.hpp"
#include "ObstacleSpawner.hpp"
#include "CollisionManager.hpp"
#include "ScoreManager.hpp"
#include "CoinManager.hpp"
#include "ShopManager.hpp"
#include "ItemSpawner.hpp"
#include "WindForce.hpp"
#include "KipasSatay.hpp"

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

    Sound sfxHit = LoadSound("assets/hit.wav");
    SetSoundVolume(sfxHit, 0.8f);

    Sound sfxItem = LoadSound("assets/pickup.wav");
    SetSoundVolume(sfxItem, 0.9f);

    // Framerate per second
    SetTargetFPS(60); 

    Background bg("assets/background2.png", screenWidth, screenHeight, 30.0f);

    WauBulan wau(screenWidth / 2.0f, screenHeight - 600.0f, "assets/waubulan.png");
    SwingingKid kid(wau.pos, "assets/kid_swinging.png", "assets/kid_falling.png", "assets/kid_standing.png");
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

    // Set up the menu positions
    kid.isOnGround = true;
    kid.pos = { (float)screenWidth / 2.0f, (float)screenHeight - 50.0f }; // Rest on ground
    wau.pos = { (float)screenWidth / 2.0f, (float)screenHeight - 600.0f }; // Hover in sky

    // Main Game Loop
    // WindowShouldClose() returns true if pressing escape or close buton
    while (!WindowShouldClose()) { 
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();
        
        float dt = GetFrameTime();

        UpdateMusicStream(bgm);

        canvas.UpdateScaling();
        Vector2 virtualMousePos = canvas.GetVirtualMousePosition();

        if (gameState == MENU) {
            bool shopHovered = GameUI::IsShopButtonClicked(screenWidth, screenHeight, virtualMousePos);

            if (shopHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                gameState = SHOP;
            }
            // Check if player clicks elsewhere to start the game
            else if (IsKeyPressed(KEY_SPACE) || (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !shopHovered)) {
                gameState = PLAYING;
                kid.isOnGround = false;
                PlayMusicStream(bgm);
                
                // Kite Stats
                KiteProfile equipped = shopManager.GetEquippedKite();
                wau.speed = equipped.speed;
                wau.radius = 25.0f * equipped.sizeMultiplier;
            }
        }
        else if (gameState == SHOP) {
            bool returnToMenu = false;
            // Pass in totalCoins from your CoinManager
            shopManager.Update(coinManager.totalCoins, returnToMenu); 
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

            // Check obstacle collisions only if the string is still attached
            if (!kid.isDetached && wau.invincibleTimer <= 0.0f && CollisionManager::CheckPlayerCollisions(wau, kid, spawner)) {
                PlaySound(sfxHit);
                kid.isDetached = true;
                
                // Give the kid a little visual bump when hitting an obstacle to emphasize the impact
                kid.velocity.y = -300.0f; 
                kid.velocity.x = (kid.pos.x < wau.pos.x) ? -150.0f : 150.0f;
            }

            // If the kid is falling, let the player click to create a new string and save him
            if (kid.isDetached && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && scoreManager.stringCharges > 0) {
                
                // We just use the virtualMousePos calculated at the top of the frame!
                if (CheckCollisionPointCircle(virtualMousePos, wau.pos, wau.radius * 3.0f)) {
                    scoreManager.stringCharges--;
                    kid.isDetached = false;
                    wau.invincibleTimer = 2.0f;
                }
            }

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
                bg.Reset();
                wau.pos = { (float)screenWidth / 2.0f, (float)screenHeight - 600.0f };

                kid.pos = { wau.pos.x, wau.pos.y + 120.0f };
                kid.velocity = { 0.0f, 0.0f };
                kid.isDetached = false;

                spawner.Reset();
                itemSpawner.Reset();
                scoreManager.Reset();
                coinManager.Reset();
                wind.Reset();
                kipas.Reset();

                // Reset to main menu positions
                kid.isDetached = false;
                kid.isOnGround = true;
                kid.velocity = { 0.0f, 0.0f };
                kid.pos = { (float)screenWidth / 2.0f, (float)screenHeight - 50.0f };
                wau.pos = { (float)screenWidth / 2.0f, (float)screenHeight - 600.0f };
            }
        }

        // Drawing logic
        canvas.BeginMode();
            if (gameState == SHOP) {
                shopManager.Draw(screenWidth, screenHeight, coinManager.totalCoins);
            }
            else {
                ClearBackground(SKYBLUE);
                bg.Draw();

                if (gameState == MENU) {
                    wau.Draw();
                    kid.Draw(wau.pos);
                    GameUI::DrawMainMenu(screenWidth, screenHeight, virtualMousePos);
                }
                else if (gameState == PLAYING) {
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
    spawner.Unload();
    itemSpawner.Unload();
    kipas.Unload();

    UnloadSound(sfxHit);
    UnloadSound(sfxItem);
    UnloadMusicStream(bgm);
    CloseAudioDevice();
    CloseWindow(); 

    return 0;
}

