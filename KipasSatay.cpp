#include "KipasSatay.hpp"
#include <cmath>

KipasSatay::KipasSatay() {
    for (int i = 0; i < 10; i++) {
        const char* fileName = TextFormat("assets/KipasSatay/kipasSatay_%d.png", i);
        Texture2D tex = LoadTexture(fileName);
        GenTextureMipmaps(&tex);
        SetTextureFilter(tex, TEXTURE_FILTER_TRILINEAR);
        frames.push_back(tex);
    }
    Reset();
}

void KipasSatay::Update(float dt, int screenWidth, bool isActive, bool isWindFromLeft) {
    // Determine target position based on wind state and direction
    if (isActive) {
        targetX = isWindFromLeft ? 200.0f : (float)screenWidth - 200.0f;
    } else {
        targetX = isWindFromLeft ? -400.0f : (float)screenWidth + 400.0f;
    }
    
    // Smoothly tween to the target
    currentX += (targetX - currentX) * 4.0f * dt;

    // Animation Speed (Fast when blowing, slow when idle)
    frameTimer += dt;
    float currentSpeed = isActive ? 0.02f : 0.1f; 
    
    if (frameTimer >= currentSpeed) {
        frameTimer = 0.0f;
        currentFrame += animDirection;
        
        // Ping-pong loop
        if (currentFrame >= 9) {
            currentFrame = 9; 
            animDirection = -1;
        } else if (currentFrame <= 0) {
            currentFrame = 0; 
            animDirection = 1;
        }
    }
}

void KipasSatay::Draw(int screenHeight, bool isActive, bool isWindFromLeft) {
    if (!frames.empty() && frames[currentFrame].id > 0) {
        Texture2D tex = frames[currentFrame];
        
        float renderWidth = 350.0f; 
        float aspectRatio = (float)tex.height / (float)tex.width;
        float renderHeight = renderWidth * aspectRatio;

        // Flip texture if blowing from the right
        float srcWidth = isWindFromLeft ? (float)tex.width : -(float)tex.width;
        
        Rectangle source = { 0.0f, 0.0f, srcWidth, (float)tex.height };
        Rectangle dest = { currentX, (float)screenHeight / 2.0f, renderWidth, renderHeight };
        Vector2 origin = { renderWidth / 2.0f, renderHeight / 2.0f };

        // Angle the fan and apply heavy shake when active
        float baseRotation = isWindFromLeft ? 30.0f : -30.0f;
        float shake = isActive ? std::sin((float)GetTime() * 40.0f) * 5.0f : 0.0f;

        DrawTexturePro(tex, source, dest, origin, baseRotation + shake, WHITE);
    }
}

void KipasSatay::Reset() {
    currentFrame = 0;
    frameTimer = 0.0f;
    animDirection = 1;
    currentX = -400.0f; // Start hidden
    targetX = -400.0f;
}

void KipasSatay::Unload() {
    for (auto& frame : frames) {
        UnloadTexture(frame);
    }
}