#include "MenuSystem.hpp"
#include "GameUI.hpp"
#include <cmath>

GameState MenuSystem::Update(float dt, int screenWidth, int screenHeight, Vector2 virtualMousePos,
                             float& menuIntroTimer, bool& introFinished, Background& menuBg,
                             WauBulan& wau, SwingingKid& kid, float& currentStringLength,
                             ShopManager& shopManager, AudioManager& audio, RewindSystem& rewindSystem) {
    // 1. Update background
    menuBg.UpdateInfinite(dt);

    // 2. Intro Rising Animation
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
    // 3. UI Interactions (Only after rising finishes)
    else {
        // Idle floating animation
        wau.pos.y = (screenHeight - 600.0f) + std::sin(GetTime() * 3.0f) * 10.0f;
        kid.pos.y = wau.pos.y + currentStringLength;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (GameUI::IsStartButtonClicked(screenWidth, screenHeight, virtualMousePos)) {
                audio.PlayGameBGM();
                
                KiteProfile equipped = shopManager.GetEquippedKite();
                wau.speed = equipped.speed;
                wau.radius = 25.0f * equipped.sizeMultiplier;
                wau.ChangeTexture(equipped.texturePath.c_str());
                currentStringLength = equipped.stringLength; 
                kid.stringLength = currentStringLength;
                
                // Drop them down to the correct gameplay starting position
                wau.pos.x = (float)screenWidth / 2.0f;
                wau.pos.y = screenHeight + 200.0f; 
                kid.pos.x = wau.pos.x;
                kid.pos.y = wau.pos.y + currentStringLength;

                rewindSystem.PrepareRising();
                return RISING;
            } 
            else if (GameUI::IsShopButtonClicked(screenWidth, screenHeight, virtualMousePos)) {
                return SHOP;
            }
        }
    }

    return MENU;
}