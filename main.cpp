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
#include "ParticleManager.hpp"
#include "ItemSpawner.hpp"
#include "WindForce.hpp"
#include "KipasSatay.hpp"
#include <cmath>

enum GameState {
    MENU,
    SHOP,
    PLAYING,
    GAMEOVER,
    VICTORY,
    WAITING_TO_REWIND,
    REWINDING,
    FALLING_DOWN,
    RISING
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
    Texture2D frozenBg = LoadTexture("assets/frozen_bg.png");

    VirtualCanvas canvas(screenWidth, screenHeight);
    ObstacleSpawner spawner(screenWidth, screenHeight);
    ItemSpawner itemSpawner(screenWidth, screenHeight);
    ScoreManager scoreManager;
    CoinManager coinManager;
    ShopManager shopManager;
    ParticleManager particleManager;
    WindForce wind;
    KipasSatay kipas;

    float currentStringLength = shopManager.GetEquippedKite().stringLength;
    WauBulan wau(screenWidth / 2.0f, screenHeight - 600.0f, "assets/waubulan.png");
    SwingingKid kid(wau.pos, "assets/kid_swinging.png", "assets/kid_falling.png", currentStringLength);

    const float NORMAL_BG_SPEED = 30.0f;
    const float BOOST_BG_SPEED = 150.0f;

    // Initialize State Machine
    GameState gameState = MENU;

    float introTimer = 0.0f;
    bool introFinished = false;
    bool skipFall = false;

    float hitStopTimer = 0.0f;

    // Set up the menu positions
    wau.pos = { (float)screenWidth / 2.0f - 250.0f, (float)screenHeight + 200.0f };
    kid.pos = { wau.pos.x, wau.pos.y + currentStringLength };

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
                    kid.pos.y = wau.pos.y + currentStringLength;
                } else {
                    wau.pos.y = endY;
                    kid.pos.y = wau.pos.y + currentStringLength;
                    introFinished = true;
                }
            } 
            // UI Interactions (Only after rising finishes)
            else {
                wau.pos.y = (screenHeight - 600.0f) + std::sin(GetTime() * 3.0f) * 10.0f;
                kid.pos.y = wau.pos.y + currentStringLength;

                bool startHovered = GameUI::IsStartButtonClicked(screenWidth, screenHeight, virtualMousePos);
                bool shopHovered = GameUI::IsShopButtonClicked(screenWidth, screenHeight, virtualMousePos);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (startHovered) {
                        gameState = PLAYING;

                        audio.PlayGameBGM();
                        
                        KiteProfile equipped = shopManager.GetEquippedKite();
                        wau.speed = equipped.speed;
                        wau.radius = 25.0f * equipped.sizeMultiplier;
                        wau.ChangeTexture(equipped.texturePath.c_str());
                        currentStringLength = equipped.stringLength; 
                        kid.stringLength = currentStringLength;
                        
                        // Drop them down to the correct gameplay starting position
                        wau.pos.x = (float)screenWidth / 2.0f;
                        wau.pos.y = screenHeight - 600.0f; 
                        kid.pos.x = wau.pos.x;
                        kid.pos.y = wau.pos.y + currentStringLength;
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
            if (hitStopTimer > 0.0f) {
                // Freeze frame: Decrement timer, skip all game logic updates
                hitStopTimer -= GetFrameTime();
            } else {
                float envDt = itemSpawner.IsSlowMoActive() ? (dt * 0.1f) : dt; // 50% speed reduction

                // Apply boost speed if active
                bg.scrollSpeed = itemSpawner.IsBoostActive() ? BOOST_BG_SPEED : NORMAL_BG_SPEED;
                bg.Update(envDt);

                if (!kid.isDetached) wau.Update(dt, screenWidth, screenHeight);
                else wau.pos.y += 400.0f * dt;
                
                bool windWasActive = wind.IsActive();
                wind.Update(envDt);
                if (!windWasActive && wind.IsActive()) {
                    audio.PlayWind(); 
                }

                kipas.Update(envDt, screenWidth, wind.IsActive(), wind.IsWindFromLeft(), audio);
                kid.Update(dt, wau.pos, wind.GetForce());
                scoreManager.Update(dt, kid.isDetached, itemSpawner.IsBoostActive());
                spawner.Update(envDt, scoreManager.currentAltitude, bg.scrollSpeed, audio);
                itemSpawner.Update(dt, envDt);
                particleManager.Update(dt);

                int coinsBefore = coinManager.totalCoins; // Check balance before collisions

                CollisionManager::HandleItemCollections(wau, kid, itemSpawner, scoreManager, coinManager, audio);

                // If the balance went up, trigger the burst at the kite's position
                if (coinManager.totalCoins > coinsBefore) {
                    particleManager.EmitCoinBurst(wau.pos);
                }

                // Check obstacle collisions
                if (!kid.isDetached && wau.invincibleTimer <= 0.0f && CollisionManager::CheckPlayerCollisions(wau, kid, spawner)) {
                    audio.PlayHit();
                    kid.Detach(wau.pos.x);
                    hitStopTimer = 0.1f;
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
                    // Check if they fall before progressing past 1 screen height
                    skipFall = (scoreManager.currentAltitude < 160.0f);
                    gameState = WAITING_TO_REWIND;
                    introTimer = 0.0f;
                    audio.StopWind();
                }

                if (bg.IsAtTop()) {
                    gameState = VICTORY;
                    audio.StopWind();
                    audio.StopBGM();
                    audio.PlayVictoryBGM();
                }
            }
        }
        else if (gameState == WAITING_TO_REWIND) {
            // Let the background and kite coast naturally for 1.5 seconds
            bg.Update(dt);
            wau.pos.y += 400.0f * dt;
            spawner.Update(dt, scoreManager.currentAltitude, bg.scrollSpeed, audio);
            itemSpawner.Update(dt, dt);
            particleManager.Update(dt);
            
            introTimer += dt;
            if (introTimer >= 0.5f) { 
                gameState = REWINDING;
                
                // Instantly clear the board so nothing is on screen during the rewind
                spawner.Reset();
                itemSpawner.Reset();
            }
        }
        else if (gameState == REWINDING) {
            float rewindSpeed = 3000.0f; 
            bg.scrollSpeed = -rewindSpeed;
            bg.Update(dt);
            
            scoreManager.currentAltitude -= rewindSpeed * dt;
            if (scoreManager.currentAltitude < 0.0f) {
                scoreManager.currentAltitude = 0.0f;
            }
            
            // REMOVED spawner and itemSpawner updates here! They are now invisible.

            float startPos = (float)screenHeight - bg.drawHeight;
            if (scoreManager.currentAltitude <= 0.0f && bg.scrollY <= startPos) {
                scoreManager.currentAltitude = 0.0f;
                
                wind.Reset();
                kipas.Reset();
                scoreManager.stringCharges = 3;
                
                bg.Reset(); 
                
                KiteProfile equipped = shopManager.GetEquippedKite();
                wau.speed = equipped.speed;
                wau.radius = 25.0f * equipped.sizeMultiplier;
                wau.ChangeTexture(equipped.texturePath.c_str());
                currentStringLength = equipped.stringLength; 
                kid.stringLength = currentStringLength;
                
                introTimer = 0.0f; 
                
                if (skipFall) {
                    wau.pos.y = screenHeight + 200.0f;
                    kid.pos.x = wau.pos.x;
                    kid.pos.y = wau.pos.y + currentStringLength;
                    
                    kid.isDetached = false;
                    gameState = RISING;
                } else {
                    wau.pos.y = -200.0f;
                    kid.pos.y = -400.0f; 
                
                    kid.isDetached = true;
                    gameState = FALLING_DOWN;
                }
            }
        }
        else if (gameState == FALLING_DOWN) {
            introTimer += dt;
            
            float fallDuration = 2.5f; 
            float wauStartY = -200.0f;
            float kidStartY = -400.0f;
            float endY = screenHeight + 200.0f; 

            if (introTimer < fallDuration) {
                float t = introTimer / fallDuration;
                float easeIn = t * t; 
                
                wau.pos.y = wauStartY + (endY - wauStartY) * easeIn;
                kid.pos.y = kidStartY + (endY - kidStartY) * easeIn;
            } else {
                wau.pos.y = endY;
                kid.pos.y = wau.pos.y + currentStringLength;
                
                kid.pos.x = wau.pos.x; 
                
                kid.isDetached = false; 
                
                gameState = RISING;
                introTimer = 0.0f; 
            }
        }
        else if (gameState == RISING) {
            //bg.scrollSpeed = NORMAL_BG_SPEED;
            bg.Update(dt);
            
            introTimer += dt;
            
            float waitDuration = 1.0f;     // 1-second pause at the bottom
            float flightDuration = 1.5f;   // 1.5-second cinematic rise
            float totalDuration = waitDuration + flightDuration;
            
            float startY = screenHeight + 200.0f;
            float endY = screenHeight - 600.0f;

            // Phase 1: Wait hidden at the bottom of the screen
            if (introTimer < waitDuration) {
                wau.pos.y = startY;
                kid.pos.y = wau.pos.y + currentStringLength;
            }
            // Phase 2: Smoothly fly up from the bottom of the screen
            else if (introTimer < totalDuration) {
                float t = (introTimer - waitDuration) / flightDuration;
                float easeOut = 1.0f - (1.0f - t) * (1.0f - t); 
                
                wau.pos.y = startY + (endY - startY) * easeOut;
                kid.pos.y = wau.pos.y + currentStringLength;
            } 
            // Phase 3: Give control back
            else {
                wau.pos.y = endY;
                kid.pos.y = wau.pos.y + currentStringLength;
                gameState = PLAYING;
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
                    kid.pos.y = wau.pos.y + currentStringLength;
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
                        // Draw the cracked ice background texture over the screen
                        Rectangle source = { 0.0f, 0.0f, (float)frozenBg.width, (float)frozenBg.height };
                        Rectangle dest = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
                        DrawTexturePro(frozenBg, source, dest, { 0.0f, 0.0f }, 0.0f, Fade(WHITE, 0.3f));

                        // Creates a chill frost overlay using Raylib's Fade function
                        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(SKYBLUE, 0.25f));
                        DrawRectangleLinesEx({0, 0, (float)screenWidth, (float)screenHeight}, 10.0f, Fade(WHITE, 0.4f));
                    }
                }
                else if (gameState == REWINDING) {

                }
                else if (gameState == FALLING_DOWN || gameState == RISING) {
                    wau.Draw();
                    kid.Draw(wau.pos, wau.invincibleTimer);
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
    UnloadTexture(frozenBg);
    audio.Unload();
    CloseAudioDevice();
    CloseWindow(); 

    return 0;
}

