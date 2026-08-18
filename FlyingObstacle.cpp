#include "FlyingObstacle.hpp"

FlyingObstacle::FlyingObstacle(int screenWidth, int screenHeight, bool flyLeft) {
    this->flyingLeft = flyLeft;
    rec.width = 60.0f;
    rec.height = 30.0f;
    rec.y = (float)GetRandomValue(50, screenHeight / 2);
    
    if (flyLeft) {
        rec.x = (float)screenWidth + 50.0f;
        speedX = -250.0f;
    } else {
        rec.x = -50.0f;
        speedX = 250.0f;
    }
}
void FlyingObstacle::Update(float dt) { rec.x += speedX * dt; }

void FlyingObstacle::Draw() { DrawRectangleRec(rec, ORANGE); }

bool FlyingObstacle::IsOffScreen(int screenWidth, int /*screenHeight*/) {
    if (flyingLeft) return rec.x + rec.width < 0.0f;
    return rec.x > screenWidth;
}