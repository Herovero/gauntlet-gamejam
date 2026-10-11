#include "raylib.h"
#include "GameState.hpp"
#include "RewindSystem.hpp"
#include "GameplaySystem.hpp"
#include "RenderSystem.hpp"
#include "GameManager.hpp"
#include "AudioManager.hpp"
#include "WauBulan.hpp"
#include "GameUI.hpp"
#include "VirtualCanvas.hpp"
#include "SwingingKid.hpp"
#include "Background.hpp"
#include "ObstacleSpawner.hpp"
#include "ScoreManager.hpp"
#include "CoinManager.hpp"
#include "ShopManager.hpp"
#include "ParticleManager.hpp"
#include "ItemSpawner.hpp"
#include "WindForce.hpp"
#include "KipasSatay.hpp"
#include <cmath>

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Wau Bulan Rising");
    SetExitKey(KEY_NULL);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60); 

    InitAudioDevice();
    AudioManager audio;
    audio.PlayMenuBGM();

    Background bg("assets/background2.png", screenWidth, screenHeight, 30.0f);
    Background menuBg("assets/skybackground.png", screenWidth, screenHeight, 150.0f);
    Texture2D gameTitle = LoadTexture("assets/game_title.png");
    Texture2D frozenBg = LoadTexture("assets/frozen_bg.png");

    RewindSystem rewindSystem;
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

    GameState gameState = MENU;
    float menuIntroTimer = 0.0f;
    bool introFinished = false;
    float hitStopTimer = 0.0f;

    wau.pos = { (float)screenWidth / 2.0f - 250.0f, (float)screenHeight + 200.0f };
    kid.pos = { wau.pos.x, wau.pos.y + currentStringLength };

    while (!WindowShouldClose()) { 
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();
        
        float dt = GetFrameTime();
        audio.Update();
        canvas.UpdateScaling();
        Vector2 virtualMousePos = canvas.GetVirtualMousePosition();

        if (gameState == MENU) {
            menuBg.UpdateInfinite(dt);

            if (!introFinished) {
                menuIntroTimer += dt;
                float duration = 3.0f; 
                float startY = screenHeight + 200.0f;
                float endY = screenHeight - 600.0f;

                if (menuIntroTimer < duration) {
                    float t = menuIntroTimer / duration;
                    float easeOut = 1.0f - (1.0f - t) * (1.0f - t); 
                    wau.pos.y = startY + (endY - startY) * easeOut;
                    kid.pos.y = wau.pos.y + currentStringLength;
                } else {
                    wau.pos.y = endY;
                    kid.pos.y = wau.pos.y + currentStringLength;
                    introFinished = true;
                }
            } 
            else {
                wau.pos.y = (screenHeight - 600.0f) + std::sin(GetTime() * 3.0f) * 10.0f;
                kid.pos.y = wau.pos.y + currentStringLength;

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (GameUI::IsStartButtonClicked(screenWidth, screenHeight, virtualMousePos)) {
                        gameState = PLAYING;
                        audio.PlayGameBGM();
                        
                        KiteProfile equipped = shopManager.GetEquippedKite();
                        wau.speed = equipped.speed;
                        wau.radius = 25.0f * equipped.sizeMultiplier;
                        wau.ChangeTexture(equipped.texturePath.c_str());
                        currentStringLength = equipped.stringLength; 
                        kid.stringLength = currentStringLength;
                        
                        wau.pos.x = (float)screenWidth / 2.0f;
                        wau.pos.y = screenHeight - 600.0f; 
                        kid.pos.x = wau.pos.x;
                        kid.pos.y = wau.pos.y + currentStringLength;
                    } 
                    else if (GameUI::IsShopButtonClicked(screenWidth, screenHeight, virtualMousePos)) {
                        gameState = SHOP;
                    }
                }
            }
        }
        else if (gameState == SHOP) {
            bool returnToMenu = false;
            int coinsBefore = coinManager.totalCoins;
            shopManager.Update(coinManager.totalCoins, returnToMenu, dt, screenWidth, virtualMousePos);

            if (coinManager.totalCoins < coinsBefore) audio.PlayPurchase();
            if (returnToMenu) gameState = MENU;
        }
        else if (gameState == PLAYING) {
            // Moved to GameplaySystem
            gameState = GameplaySystem::Update(dt, screenWidth, screenHeight, virtualMousePos, 
                                               hitStopTimer, bg, wau, kid, wind, kipas, scoreManager, 
                                               spawner, itemSpawner, particleManager, coinManager, 
                                               audio, rewindSystem);
        }
        else if (gameState == WAITING_TO_REWIND) {
            particleManager.Update(dt);
            gameState = rewindSystem.UpdateWaiting(dt, bg, wau, spawner, itemSpawner, scoreManager, audio);
        }
        else if (gameState == REWINDING) {
            gameState = rewindSystem.UpdateRewinding(dt, bg, scoreManager, wind, kipas, shopManager, wau, kid, currentStringLength, screenHeight);
        }
        else if (gameState == FALLING_DOWN) {
            gameState = rewindSystem.UpdateFalling(dt, wau, kid, currentStringLength, screenHeight);
        }
        else if (gameState == RISING) {
            gameState = rewindSystem.UpdateRising(dt, bg, wau, kid, currentStringLength, screenHeight);
        }
        else if (gameState == PAUSED || gameState == VICTORY) {
            bool resumeHovered = GameUI::IsRestartButtonClicked(screenWidth, screenHeight, virtualMousePos);
            bool menuHovered = GameUI::IsMainMenuButtonClicked(screenWidth, screenHeight, virtualMousePos);

            if (gameState == PAUSED && (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P))) {
                gameState = PLAYING;
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (resumeHovered) {
                    gameState = PLAYING;
                    if (gameState == VICTORY) {
                        audio.PlayGameBGM();
                        GameManager::ResetGame(screenWidth, screenHeight, wau, kid, bg, spawner, itemSpawner, scoreManager, coinManager, wind, kipas);
                        KiteProfile equipped = shopManager.GetEquippedKite();
                        wau.speed = equipped.speed;
                        wau.radius = 25.0f * equipped.sizeMultiplier;
                        wau.ChangeTexture(equipped.texturePath.c_str());
                        wau.pos.y = screenHeight - 600.0f; 
                        kid.pos.y = wau.pos.y + currentStringLength;
                    }
                }
                else if (menuHovered) {
                    gameState = MENU;
                    audio.PlayMenuBGM();
                    GameManager::ResetGame(screenWidth, screenHeight, wau, kid, bg, spawner, itemSpawner, scoreManager, coinManager, wind, kipas);
                    menuIntroTimer = 0.0f;
                    introFinished = false;
                    wau.Reset((float)screenWidth / 2.0f - 250.0f, (float)screenHeight + 200.0f); 
                    kid.pos = { wau.pos.x, wau.pos.y + 550.0f };
                }
            }
        }

        // Moved to RenderSystem
        canvas.BeginMode();
        RenderSystem::Draw(gameState, screenWidth, screenHeight, virtualMousePos, introFinished, 
                           gameTitle, frozenBg, bg, menuBg, wau, kid, shopManager, coinManager, 
                           wind, kipas, spawner, itemSpawner, particleManager, scoreManager);
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