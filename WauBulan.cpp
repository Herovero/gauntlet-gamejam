#include "WauBulan.hpp"
#include <cmath>

WauBulan::WauBulan(float startX, float startY, const char* texturePath) {
    pos = { startX, startY };
    radius = 25.0f;
    speed = 250.0f;

    texture = LoadTexture(texturePath);
    spriteWidth = 200.0f;
    spriteHeight = 100.0f;
}

void WauBulan::Update(float dt, int screenWidth, int screenHeight) {
    // Movement
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) pos.x -= speed * dt;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) pos.x += speed * dt;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) pos.y -= speed * dt;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) pos.y += speed * dt;
    
    // Horizontal boundaries
    if (pos.x < radius) pos.x = radius;
    if (pos.x > screenWidth - radius) pos.x = screenWidth - radius;
        
    // Vertical boundaries
    if (pos.y < radius) pos.y = radius;
    if (pos.y > screenHeight - radius) pos.y = screenHeight - radius;
}

void WauBulan::Draw() {
    float time = (float)GetTime();

    // Wind Drft
    float windSway = std::sin(time * 4.0f) * 6.0f;
    Vector2 renderPos = { pos.x + windSway, pos.y };

    // Idle animation (oscillates between -3.0 and +3.0 degrees)
    float windTilt = std::sin(time * 6.0f) * 3.0f;

    Rectangle sourceRec = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle destRec   = { renderPos.x, renderPos.y, spriteWidth, spriteHeight };
    Vector2 origin      = { spriteWidth / 2.0f, spriteHeight / 2.0f };

    // DrawTexturePro(Texture2D texture, Rectangle srcrec, Rectangle dstrec, Vector2 origin, float rotation, Color tint)
    DrawTexturePro(texture, sourceRec, destRec, origin, windTilt, WHITE);

    // Collision hitbox debug
    DrawCircleLines((int)renderPos.x, (int)renderPos.y, radius, LIME);
}

void WauBulan::Unload() {
    UnloadTexture(texture);
}