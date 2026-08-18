#pragma once
#include "Obstacle.hpp"

class FlyingObstacle : public Obstacle {
public:
    float speedX;
    bool flyingLeft;
    Texture2D texture;

    FlyingObstacle(int screenWidth, int screenHeight, bool flyLeft, Texture2D texture);
    void Update(float dt) override;
    void Draw() override;
    bool IsOffScreen(int screenWidth, int screenHeight) override;
};