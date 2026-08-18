#pragma once
#include "Obstacle.hpp"

class FlyingObstacle : public Obstacle {
public:
    float speedX;
    bool flyingLeft;
    FlyingObstacle(int screenWidth, int screenHeight, bool flyLeft);
    void Update(float dt) override;
    void Draw() override;
    bool IsOffScreen(int screenWidth, int screenHeight) override;
};