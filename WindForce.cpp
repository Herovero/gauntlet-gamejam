#include "WindForce.hpp"
#include <cmath>

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
            activeWindTimer = (float)GetRandomValue(3, 6); // Blow for multiple seconds
            nextWindTimer = (float)GetRandomValue(12, 20); // Wait for several seconds
            
            // Pick a direction (2500 is strong enough to counter your 1200 gravity)
            targetWindForce = (GetRandomValue(0, 1) == 0) ? -2500.0f : 2500.0f; 
        }
    }
}

void WindForce::Draw(int screenWidth) {
    if (activeWindTimer > 0.0f) {
        const char* windWarning = (targetWindForce > 0.0f) ? ">> STRONG WIND >>" : "<< STRONG WIND <<";
        
        int fontSize = 50;
        int textWidth = MeasureText(windWarning, fontSize);
        
        // Calculate smooth fading using a sine wave
        // GetTime() drives the animation. Multiply by 8.0f to control the pulse speed.
        float time = (float)GetTime();
        
        // std::sin returns a value between -1.0 and 1.0. 
        // We normalize it to a range between 0.0 and 1.0 for the alpha transparency.
        float pulse = (std::sin(time * 8.0f) + 1.0f) / 2.0f; 
        
        // Keep it slightly visible at its dimmest point (e.g., minimum 20% opacity)
        float alpha = 0.2f + (pulse * 0.8f); 

        // Apply the fading alpha to the RED color
        Color pulsingColor = Fade(RED, alpha);

        // Draw the text slightly lower so it doesn't clip the top of the screen
        DrawText(windWarning, screenWidth / 2 - textWidth / 2, 120, fontSize, pulsingColor);
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