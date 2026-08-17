#include "Obstacle.hpp"

Obstacle::Obstacle(int screenWidth) {
    fallSpeed = 150.0f;
    Reset(screenWidth);
}

// Drop obstacle
void Obstacle::Update(float dt, int screenWidth, int screenHeight) {
    rec.y += fallSpeed * dt;
    if (rec.y > screenHeight) {
        Reset(screenWidth);
    }
}

// After obstacle reaches bottom, go back to the top and randomize x position
void Obstacle::Reset(int screenWidth) {
    rec.width = 150.0f;
    rec.height = 30.0f;
    rec.y = -50.0f;
    rec.x = (float)GetRandomValue(0, screenWidth - (int)rec.width);
}

void Obstacle::Draw() {
    DrawRectangleRec(rec, RED);
}