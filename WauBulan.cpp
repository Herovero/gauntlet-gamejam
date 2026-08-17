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
    float windSway = std::sin(GetTime() * 4.0f) * 6.0f;
    Vector2 renderPos = { pos.x + windSway, pos.y };

    Rectangle sourceRec = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle destRec   = { renderPos.x, renderPos.y, spriteWidth, spriteHeight };
    Vector2 origin      = { spriteWidth / 2.0f, spriteHeight / 2.0f };

    // DrawCircleV(renderPos, radius, BLACK);
    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
}

void WauBulan::Unload() {
    UnloadTexture(texture);
}