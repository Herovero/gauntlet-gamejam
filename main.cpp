#include "raylib.h"
#include "GameState.hpp"
#include "RewindSystem.hpp"
#include "GameplaySystem.hpp"
#include "MenuSystem.hpp"
#include "OverlaySystem.hpp"
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
#include "ShopSystem.hpp"
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
            gameState = MenuSystem::Update(dt, screenWidth, screenHeight, virtualMousePos,
                                           menuIntroTimer, introFinished, menuBg, wau, kid,
                                           currentStringLength, shopManager, audio, 
                                           rewindSystem);
        }
        else if (gameState == SHOP) {
            gameState = ShopSystem::Update(dt, screenWidth, virtualMousePos, shopManager, 
                                           coinManager, audio, wau, kid, currentStringLength);
        }
        else if (gameState == PLAYING) {
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
            gameState = OverlaySystem::Update(gameState, screenWidth, screenHeight, virtualMousePos, 
                                              audio, wau, kid, bg, spawner, itemSpawner, scoreManager, 
                                              coinManager, wind, kipas, shopManager, menuIntroTimer, 
                                              introFinished, currentStringLength, rewindSystem);
        }

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