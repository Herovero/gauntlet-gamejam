#include "SwayingObstacle.hpp"
#include <cmath>

SwayingObstacle::SwayingObstacle(int screenWidth, Texture2D tex) {
    texture = tex;
    rec.width = 45.0f;
    rec.height = 45.0f;
    baseX = (float)GetRandomValue(120, screenWidth - 120);
    rec.x = baseX;
    rec.y = -50.0f;
    speedY = 100.0f;
    swayOffset = (float)GetRandomValue(0, 100);
}

void SwayingObstacle::Update(float dt) {
    rec.y += speedY * dt;
    rec.x = baseX + std::sin((float)GetTime() * 3.0f + swayOffset) * 120.0f;
}

void SwayingObstacle::Draw() {
    if (texture.id > 0) {
        Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
        DrawTexturePro(texture, source, rec, { 0, 0 }, 0.0f, WHITE);
    } else {
        DrawRectangleRec(rec, PURPLE);
    }
}

bool SwayingObstacle::IsOffScreen(int /*screenWidth*/, int screenHeight) { return rec.y > screenHeight; }