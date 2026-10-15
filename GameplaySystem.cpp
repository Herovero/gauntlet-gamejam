#include "GameplaySystem.hpp"
#include "CollisionManager.hpp"
#include "GameUI.hpp"

GameState GameplaySystem::Update(float dt, int screenWidth, int screenHeight, Vector2 virtualMousePos,
                                 float& hitStopTimer, Background& bg, WauBulan& wau, SwingingKid& kid,
                                 WindForce& wind, KipasSatay& kipas, ScoreManager& scoreManager,
                                 ObstacleSpawner& spawner, ItemSpawner& itemSpawner,
                                 ParticleManager& particleManager, CoinManager& coinManager,
                                 AudioManager& audio, RewindSystem& rewindSystem) {
    // Pause check
    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE) || 
   (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && GameUI::IsPauseButtonClicked(screenWidth, virtualMousePos))) {
        audio.StopWind();
        wau.ResetJoystick();
        return PAUSED;
    }

    // Freeze frame hit-stop logic
    if (hitStopTimer > 0.0f) {
        hitStopTimer -= dt;
        return PLAYING;
    }

    const float NORMAL_BG_SPEED = 30.0f;
    const float BOOST_BG_SPEED = 150.0f;

    float envDt = itemSpawner.IsSlowMoActive() ? (dt * 0.1f) : dt;

    bg.scrollSpeed = itemSpawner.IsBoostActive() ? BOOST_BG_SPEED : NORMAL_BG_SPEED;
    bg.Update(envDt);

    if (!kid.isDetached) wau.Update(envDt, screenWidth, screenHeight);
    else wau.pos.y += 400.0f * envDt;
    
    bool windWasActive = wind.IsActive();
    float currentAltitude = scoreManager.currentAltitude;
    wind.Update(dt, currentAltitude);
    if (!windWasActive && wind.IsActive()) audio.PlayWind(); 

    kipas.Update(envDt, screenWidth, wind.IsActive(), wind.IsWindFromLeft(), audio);
    kid.Update(envDt, wau.pos, wind.GetForce());
    scoreManager.Update(envDt, kid.isDetached, itemSpawner.IsBoostActive());
    spawner.Update(envDt, scoreManager.currentAltitude, bg.scrollSpeed, audio);
    itemSpawner.Update(dt, envDt, currentAltitude);
    particleManager.Update(dt);

    int coinsBefore = coinManager.totalCoins; 
    CollisionManager::HandleItemCollections(wau, kid, itemSpawner, scoreManager, coinManager, audio);

    if (coinManager.totalCoins > coinsBefore) {
        particleManager.EmitCoinBurst(wau.pos);
    }

    if (!kid.isDetached && wau.invincibleTimer <= 0.0f && CollisionManager::CheckPlayerCollisions(wau, kid, spawner)) {
        audio.PlayHit();
        kid.Detach(wau.pos.x);
        hitStopTimer = 0.1f;

        // If the player stores an Ais Kacang, activate the slow-mo to help them save the kid
        if (itemSpawner.hasAisKacangBuff) {
            itemSpawner.hasAisKacangBuff = false;
            itemSpawner.TriggerSlowMo();
            audio.PlayFreeze();
        }
    }

    bool wasDetached = kid.isDetached;
    kid.TryReattach(virtualMousePos, wau.pos.x, wau.pos.y, wau.radius, wau.invincibleTimer, scoreManager.stringCharges);

    if (wasDetached && !kid.isDetached) {
        audio.PlaySnap();
        itemSpawner.CancelSlowMo(); 
    }

    // Death transition trigger
    if (kid.isDetached && (kid.pos.y - kid.radius) > (float)screenHeight) {
        rewindSystem.TriggerDeath(scoreManager.currentAltitude);
        audio.StopWind();
        return WAITING_TO_REWIND;
    }

    // Victory transition trigger
    if (bg.IsAtTop()) {
        audio.StopWind();
        audio.StopBGM();
        audio.PlayVictoryBGM();
        return VICTORY;
    }

    return PLAYING;
}