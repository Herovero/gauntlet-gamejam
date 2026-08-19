#include "TaliTangsi.hpp"

TaliTangsi::TaliTangsi(int screenWidth, Texture2D tex) {
    texture = tex;
    radius = 20.0f; 
    speedY = 150.0f;
    active = true;
    
    pos.x = (float)GetRandomValue(100, screenWidth - 100);
    pos.y = -50.0f; 
}

void TaliTangsi::Update(float dt) {
    if (active) pos.y += speedY * dt;
}

void TaliTangsi::Draw() {
    if (!active) return;

    if (texture.id > 0) {
        float renderWidth = radius * 2.0f;
        float aspectRatio = (float)texture.height / (float)texture.width;
        float renderHeight = renderWidth * aspectRatio;

        Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
        Rectangle dest   = { pos.x, pos.y, renderWidth, renderHeight };
        Vector2 origin   = { renderWidth / 2.0f, renderHeight / 2.0f };

        DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
    } else {
        DrawCircleV(pos, radius, GOLD);
    }
}

bool TaliTangsi::CheckCollision(Vector2 playerPos, float playerRadius) {
    if (!active) return false;
    return CheckCollisionCircles(pos, radius, playerPos, playerRadius);
}