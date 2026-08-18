#pragma once
#include "Obstacle.hpp"

class FallingObstacle : public Obstacle {
public:
    Vector2 pos;
    float radius;
    float speedY;
    Texture2D texture;

    FallingObstacle(int screenWidth, Texture2D tex);
    void Update(float dt) override;
    void Draw() override;
    bool IsOffScreen(int screenWidth, int screenHeight) override;
    bool CheckCollision(Vector2 playerPos, float playerRadius) override;
};