#pragma once
#include "Obstacle.hpp"

class FallingObstacle : public Obstacle {
public:
    float speedY;
    FallingObstacle(int screenWidth);
    void Update(float dt) override;
    void Draw() override;
    bool IsOffScreen(int screenWidth, int screenHeight) override;
};