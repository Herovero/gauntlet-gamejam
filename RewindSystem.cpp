#include "RewindSystem.hpp"

RewindSystem::RewindSystem() : timer(0.0f), skipFall(false) {}

void RewindSystem::TriggerDeath(float altitude) {
    skipFall = (altitude < 160.0f);
    timer = 0.0f;
}

void RewindSystem::PrepareRising() {
    timer = 0.0f;
    skipFall = true;
}

GameState RewindSystem::UpdateWaiting(float dt, Background& bg, WauBulan& wau, ObstacleSpawner& spawner, ItemSpawner& itemSpawner, ScoreManager& scoreManager, AudioManager& audio) {
    bg.Update(dt);
    wau.pos.y += 400.0f * dt;
    spawner.Update(dt, scoreManager.currentAltitude, bg.scrollSpeed, audio);
    itemSpawner.Update(dt, dt);
    
    timer += dt;
    if (timer >= 0.5f) { 
        spawner.Reset();
        itemSpawner.Reset();
        return REWINDING;
    }
    return WAITING_TO_REWIND;
}

GameState RewindSystem::UpdateRewinding(float dt, Background& bg, ScoreManager& scoreManager, WindForce& wind, KipasSatay& kipas, ShopManager& shopManager, WauBulan& wau, SwingingKid& kid, float& currentStringLength, int screenHeight) {
    float rewindSpeed = 3000.0f; 
    bg.scrollSpeed = -rewindSpeed;
    bg.Update(dt);
    
    scoreManager.currentAltitude -= rewindSpeed * dt;
    if (scoreManager.currentAltitude < 0.0f) scoreManager.currentAltitude = 0.0f;

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
        
        timer = 0.0f; 
        
        if (skipFall) {
            wau.pos.y = screenHeight + 200.0f;
            kid.pos.x = wau.pos.x;
            kid.pos.y = wau.pos.y + currentStringLength;
            kid.isDetached = false;
            return RISING;
        } else {
            wau.pos.y = -200.0f;
            kid.pos.y = -400.0f; 
            kid.isDetached = true;
            return FALLING_DOWN;
        }
    }
    return REWINDING;
}

GameState RewindSystem::UpdateFalling(float dt, WauBulan& wau, SwingingKid& kid, float currentStringLength, int screenHeight) {
    timer += dt;
    float fallDuration = 2.5f; 
    float endY = screenHeight + 200.0f; 

    if (timer < fallDuration) {
        float t = timer / fallDuration;
        float easeIn = t * t; 
        wau.pos.y = -200.0f + (endY - (-200.0f)) * easeIn;
        kid.pos.y = -400.0f + (endY - (-400.0f)) * easeIn;
        return FALLING_DOWN;
    } else {
        wau.pos.y = endY;
        kid.pos.x = wau.pos.x; 
        kid.pos.y = wau.pos.y + currentStringLength;
        kid.isDetached = false; 
        timer = 0.0f; 
        return RISING;
    }
}

GameState RewindSystem::UpdateRising(float dt, Background& bg, WauBulan& wau, SwingingKid& kid, float currentStringLength, int screenHeight) {
    wau.stretchFactor = 1.0f;
    wau.targetRotation = 0.0f;
    wau.rotation = 0.0f;

    if (!skipFall) {
        bg.Update(dt);
    }
    
    timer += dt;
    
    float waitDuration = 1.0f;     
    float totalDuration = waitDuration + 1.5f;
    float startY = screenHeight + 200.0f;
    float endY = screenHeight - 600.0f;

    if (timer < waitDuration) {
        wau.pos.y = startY;
        kid.pos.y = wau.pos.y + currentStringLength;
    } else if (timer < totalDuration) {
        float t = (timer - waitDuration) / 1.5f;
        float easeOut = 1.0f - (1.0f - t) * (1.0f - t); 
        wau.pos.y = startY + (endY - startY) * easeOut;
        kid.pos.y = wau.pos.y + currentStringLength;
    } else {
        wau.pos.y = endY;
        kid.pos.y = wau.pos.y + currentStringLength;
        return PLAYING;
    }
    return RISING;
}