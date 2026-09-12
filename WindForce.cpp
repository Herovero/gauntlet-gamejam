#include "WindForce.hpp"

WindForce::WindForce() {
    Reset();
}

void WindForce::Update(float dt) {
    if (activeWindTimer > 0.0f) {
        activeWindTimer -= dt;
        currentWindForce = targetWindForce;
    } else {
        // Ease the wind back down to 0 smoothly when it stops
        currentWindForce += (0.0f - currentWindForce) * 2.0f * dt; 
        
        nextWindTimer -= dt;
        if (nextWindTimer <= 0.0f) {
            activeWindTimer = (float)GetRandomValue(2, 4); // Blows for 2-4 seconds
            nextWindTimer = (float)GetRandomValue(12, 20); // Wait 12-20 seconds
            
            // Pick a direction (2500 is strong enough to counter your 1200 gravity)
            targetWindForce = (GetRandomValue(0, 1) == 0) ? -2500.0f : 2500.0f; 
        }
    }
}

void WindForce::Draw(int screenWidth) {
    if (activeWindTimer > 0.0f) {
        const char* windWarning = (targetWindForce > 0.0f) ? ">> STRONG WIND >>" : "<< STRONG WIND <<";
        DrawText(windWarning, screenWidth / 2 - MeasureText(windWarning, 30) / 2, 100, 30, RED);
    }
}

float WindForce::GetForce() const {
    return currentWindForce;
}

void WindForce::Reset() {
    currentWindForce = 0.0f;
    targetWindForce = 0.0f;
    activeWindTimer = 0.0f;
    nextWindTimer = 10.0f; // First wind starts after 10 seconds
}