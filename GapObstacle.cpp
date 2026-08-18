#include "GapObstacle.hpp"

GapObstacle::GapObstacle(int screenWidth) {
    float gapWidth = 250.0f;
    float gapX = (float)GetRandomValue(50, screenWidth - (int)gapWidth - 50);

    rec.x = 0.0f;
    rec.y = -50.0f;
    rec.width = gapX;
    rec.height = 40.0f;

    rec2.x = gapX + gapWidth;
    rec2.y = -50.0f;
    rec2.width = (float)screenWidth - rec2.x;
    rec2.height = 40.0f;
    
    speedY = 125.0f;
}

void GapObstacle::Update(float dt) {
    rec.y += speedY * dt;
    rec2.y += speedY * dt;
}

void GapObstacle::Draw() {
    DrawRectangleRec(rec, DARKGRAY);
    DrawRectangleRec(rec2, DARKGRAY);
}

bool GapObstacle::IsOffScreen(int /*screenWidth*/, int screenHeight) { return rec.y > screenHeight; }

bool GapObstacle::CheckCollision(Vector2 pos, float radius) {
    return CheckCollisionCircleRec(pos, radius, rec) || CheckCollisionCircleRec(pos, radius, rec2);
}