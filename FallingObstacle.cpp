#include "FallingObstacle.hpp"

FallingObstacle::FallingObstacle(int screenWidth) {
    rec.width = 150.0f;
    rec.height = 30.0f;
    rec.x = (float)GetRandomValue(0, screenWidth - (int)rec.width);
    rec.y = -50.0f;
    speedY = 150.0f;
}

void FallingObstacle::Update(float dt) { rec.y += speedY * dt; }

void FallingObstacle::Draw() { DrawRectangleRec(rec, RED); }

bool FallingObstacle::IsOffScreen(int /*screenWidth*/, int screenHeight) { return rec.y > screenHeight; }