#include "WauBulan.hpp"
#include <cmath>

#if defined(__ANDROID__)
    #include "raymob.h"
#endif

WauBulan::WauBulan(float startX, float startY, const char* texturePath) {
    pos = { startX, startY };
    radius = 25.0f;
    speed = 250.0f;
    invincibleTimer = 0.0f;

    texture = LoadTexture(texturePath);
    spriteWidth = 200.0f;
    spriteHeight = 100.0f;
    
    rotation = 0.0f;
    targetRotation = 0.0f;
    stretchFactor = 1.0f;

    GenTextureMipmaps(&texture);
    SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);
}

void WauBulan::Update(float dt, int screenWidth, int screenHeight) {
    if (invincibleTimer > 0.0f) {
        invincibleTimer -= dt;
    }

    targetRotation = 0.0f;
    stretchFactor = 1.0f;

    float currentSpeedX = 0.0f;
    float currentSpeedY = 0.0f;

    #if defined(__ANDROID__)
        if (isTiltEnabled) {
            Vector3 accel = GetAccelerotmerAxis();
            float tiltY = accel.y;
            float tiltX = accel.x;
            
            if (tiltY > 0.5f) { currentSpeedX = speed; targetRotation += 20.0f; }
            else if (tiltY < -0.5f) { currentSpeedX = -speed; targetRotation -= 20.0f; }

            if (tiltX > 1.5f) { currentSpeedY = speed; stretchFactor = 0.90f; }
            else if (tiltX < -1.5f) { currentSpeedY = -speed; stretchFactor = 1.15f; }
        } else {
            joystick.Update();
        }
    #else
        // PC Keyboard Fallback
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) { currentSpeedX = -speed; targetRotation -= 20.0f; }
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) { currentSpeedX = speed; targetRotation += 20.0f; }
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) { currentSpeedY = -speed; stretchFactor = 1.15f; }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) { currentSpeedY = speed; stretchFactor = 0.90f; }
    #endif

    if (!isTiltEnabled && joystick.IsActive()) {
        Vector2 input = joystick.GetInput();
        currentSpeedX = input.x * speed;
        currentSpeedY = input.y * speed;
        
        if (input.x > 0.1f) targetRotation += 20.0f;
        else if (input.x < -0.1f) targetRotation -= 20.0f;

        if (input.y < -0.1f) stretchFactor = 1.15f;
        else if (input.y > 0.1f) stretchFactor = 0.90f;
    }

    // Apply movement
    pos.x += currentSpeedX * dt;
    pos.y += currentSpeedY * dt;

    // Steer Animation (Smoothly interpolate current rotation to target rotation)
    rotation += (targetRotation - rotation) * 10.0f * dt;
    
    // Horizontal boundaries
    if (pos.x < radius) pos.x = radius;
    if (pos.x > screenWidth - radius) pos.x = screenWidth - radius;
        
    // Vertical boundaries
    if (pos.y < radius) pos.y = radius;
    float bottomLimit = (float)screenHeight - 300.0f;
    if (pos.y > bottomLimit) pos.y = bottomLimit;
}

void WauBulan::Draw() {
    Color tint = WHITE;

    float time = (float)GetTime();

    if (invincibleTimer > 0.0f) {
        // Flicker every 0.1 seconds
        if ((int)(invincibleTimer * 15) % 2 == 0) {
            tint = Fade(WHITE, 0.05f); // Ghostly transparent
        }
    }

    // Wind Drft
    float windSway = std::sin(time * 4.0f) * 6.0f;
    Vector2 renderPos = { pos.x + windSway, pos.y };

    // Idle animation (oscillates between -3.0 and +3.0 degrees)
    float windTilt = std::sin(time * 6.0f) * 3.0f;

    // Combine idle and steering animation
    float finalRotation = rotation + windTilt;

    float currentWidth  = spriteWidth * (2.0f - stretchFactor);
    float currentHeight = spriteHeight * stretchFactor;

    Rectangle sourceRec = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle destRec   = { renderPos.x, renderPos.y, currentWidth, currentHeight };
    Vector2 origin      = { currentWidth / 2.0f, currentHeight / 2.0f };

    // DrawTexturePro(Texture2D texture, Rectangle srcrec, Rectangle dstrec, Vector2 origin, float rotation, Color tint)
    DrawTexturePro(texture, sourceRec, destRec, origin, finalRotation, tint);

    // Collision hitbox debug
    //DrawCircleLines((int)renderPos.x, (int)renderPos.y, radius, LIME);

    if (!isTiltEnabled) {
        joystick.Draw();
    }
}

void WauBulan::Reset(float startX, float startY) {
    pos.x = startX;
    pos.y = startY;
    
    rotation = 0.0f;
    targetRotation = 0.0f;
    stretchFactor = 1.0f; 
    
    invincibleTimer = 0.0f;
}

void WauBulan::ChangeTexture(const char* newTexturePath) {
    // Free the old image from memory
    UnloadTexture(texture);
    
    // Load the new image and reapply your smooth scaling filters
    texture = LoadTexture(newTexturePath);
    GenTextureMipmaps(&texture);
    SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);
}

void WauBulan::Unload() {
    UnloadTexture(texture);
}

void WauBulan::ResetJoystick() {
    joystick.Reset();
}