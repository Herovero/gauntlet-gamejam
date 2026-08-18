#pragma once
#include "Obstacle.hpp"

class SwayingObstacle : public Obstacle {
public:
    float speedY, baseX, swayOffset;
    SwayingObstacle(int screenWidth);
    void Update(float dt) override;
    void Draw() override;
    bool IsOffScreen(int screenWidth, int screenHeight) override;
};