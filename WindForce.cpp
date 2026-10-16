#include "WindForce.hpp"
#include <cmath>

WindForce::WindForce() {
    Reset();
}

void WindForce::Update(float dt, float currentAltitude) {
    // Prevent wind from updating/spawning entirely
    if (currentAltitude <= 250.0f) {
        return; 
    }

    if (activeWindTimer > 0.0f) {
        activeWindTimer -= dt;
        currentWindForce = targetWindForce;

        // When the wind stops blowing, prepare the NEXT wind direction and make it less frequent
        if (activeWindTimer <= 0.0f) {
            nextWindTimer = (float)GetRandomValue(25, 40); // Increased wait time to spawn less often
            targetWindForce = (GetRandomValue(0, 1) == 0) ? -2500.0f : 2500.0f; 
        }
    } else {
        // Ease the wind back down to 0 smoothly when it stops
        currentWindForce += (0.0f - currentWindForce) * 2.0f * dt; 
        
        nextWindTimer -= dt;
        if (nextWindTimer <= 0.0f) {
            activeWindTimer = (float)GetRandomValue(3, 6);
        }
    }
}

void WindForce::Draw(int screenWidth) {
    // 2. Display warning text while blowing OR during the 3-second warning phase before it blows
    if (activeWindTimer > 0.0f || (nextWindTimer > 0.0f && nextWindTimer <= 3.0f)) {
        
        // Update text to make it explicitly clear it is a warning
        const char* windWarning = (targetWindForce > 0.0f) ? ">> WARNING: STRONG WIND >>" : "<< WARNING: STRONG WIND <<";
        
        int fontSize = 50;
        int textWidth = MeasureText(windWarning, fontSize);
        
        float time = (float)GetTime();
        float pulse = (std::sin(time * 8.0f) + 1.0f) / 2.0f; 
        float alpha = 0.2f + (pulse * 0.8f); 

        // Change warning color to ORANGE when warning, RED when actively blowing
        Color textColor = (activeWindTimer > 0.0f) ? RED : ORANGE;
        Color pulsingColor = Fade(textColor, alpha);

        DrawText(windWarning, screenWidth / 2 - textWidth / 2, 120, fontSize, pulsingColor);
    }
}

float WindForce::GetForce() const {
    return currentWindForce;
}

void WindForce::Reset() {
    currentWindForce = 0.0f;
    targetWindForce = (GetRandomValue(0, 1) == 0) ? -2500.0f : 2500.0f;
    activeWindTimer = 0.0f;
    nextWindTimer = 10.0f; // First wind starts after 10 seconds
}