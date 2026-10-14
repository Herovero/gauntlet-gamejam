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

    if (!kid.isDetached) wau.Update(dt, screenWidth, screenHeight);
    else wau.pos.y += 400.0f * dt;
    
    bool windWasActive = wind.IsActive();
    wind.Update(envDt);
    if (!windWasActive && wind.IsActive()) audio.PlayWind(); 

    kipas.Update(envDt, screenWidth, wind.IsActive(), wind.IsWindFromLeft(), audio);
    kid.Update(dt, wau.pos, wind.GetForce());
    scoreManager.Update(dt, kid.isDetached, itemSpawner.IsBoostActive());
    spawner.Update(envDt, scoreManager.currentAltitude, bg.scrollSpeed, audio);
    itemSpawner.Update(dt, envDt);
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
    }

    bool wasDetached = kid.isDetached;
    kid.TryReattach(virtualMousePos, wau.pos.x, wau.pos.y, wau.radius, wau.invincibleTimer, scoreManager.stringCharges);

    if (wasDetached && !kid.isDetached) audio.PlaySnap();

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