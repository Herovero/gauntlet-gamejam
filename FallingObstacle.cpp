#include "FallingObstacle.hpp"

FallingObstacle::FallingObstacle(int screenWidth, Texture2D tex) {
    texture = tex;
    rec.width = 150.0f;
    rec.height = 30.0f;
    rec.x = (float)GetRandomValue(0, screenWidth - (int)rec.width);
    rec.y = -50.0f;
    speedY = 150.0f;
}

void FallingObstacle::Update(float dt) { rec.y += speedY * dt; }

void FallingObstacle::Draw() {
    Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    DrawTexturePro(texture, source, rec, { 0, 0 }, 0.0f, WHITE);
}

bool FallingObstacle::IsOffScreen(int /*screenWidth*/, int screenHeight) { return rec.y > screenHeight; }