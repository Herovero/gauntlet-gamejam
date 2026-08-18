#pragma once
#include "Obstacle.hpp"

class GapObstacle : public Obstacle {
public:
    Rectangle rec2;
    float speedY;
    Texture2D texture;

    GapObstacle(int screenWidth, Texture2D tex);
    void Update(float dt) override;
    void Draw() override;
    bool IsOffScreen(int screenWidth, int screenHeight) override;
    bool CheckCollision(Vector2 pos, float radius) override;
    bool IsGapType() override { return true; }
};