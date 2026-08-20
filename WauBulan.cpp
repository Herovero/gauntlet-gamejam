#include "WauBulan.hpp"
#include <cmath>

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
}

void WauBulan::Update(float dt, int screenWidth, int screenHeight) {
    if (invincibleTimer > 0.0f) {
        invincibleTimer -= dt;
    }

    targetRotation = 0.0f;
    stretchFactor = 1.0f;

    // Movement
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        pos.x -= speed * dt;
        targetRotation -= 20.0f;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        pos.x += speed * dt;
        targetRotation += 20.f;
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        pos.y -= speed * dt;
        stretchFactor = 1.15f;
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        pos.y += speed * dt;
        stretchFactor = 0.90f;
    }

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
    Color drawColor = WHITE;

    float time = (float)GetTime();

    if (invincibleTimer > 0.0f) {
        // Flicker every 0.1 seconds
        if ((int)(invincibleTimer * 10) % 2 == 0) {
            drawColor = Fade(WHITE, 0.4f); // Make it 40% opaque
        }
    }

    // Wind Drft
    float windSway = std::sin(time * 4.0f) * 6.0f;
    Vector2 renderPos = { pos.x + windSway, pos.y };

    // Idle animation (oscillates between -3.0 and +3.0 degrees)
    float windTilt = std::sin(time * 6.0f) * 3.0f;

    // Combine idle and steering animation
    float finalRotation = rotation + windTilt;

    //
    float currentWidth  = spriteWidth * (2.0f - stretchFactor);
    float currentHeight = spriteHeight * stretchFactor;

    Rectangle sourceRec = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle destRec   = { renderPos.x, renderPos.y, currentWidth, currentHeight };
    Vector2 origin      = { currentWidth / 2.0f, currentHeight / 2.0f };

    // DrawTexturePro(Texture2D texture, Rectangle srcrec, Rectangle dstrec, Vector2 origin, float rotation, Color tint)
    DrawTexturePro(texture, sourceRec, destRec, origin, finalRotation, WHITE);

    // Collision hitbox debug
    DrawCircleLines((int)renderPos.x, (int)renderPos.y, radius, LIME);
}

void WauBulan::Unload() {
    UnloadTexture(texture);
}