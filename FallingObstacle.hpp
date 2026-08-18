#pragma once
#include "Obstacle.hpp"

class FallingObstacle : public Obstacle {
public:
    float speedY;
    Texture2D texture;

    FallingObstacle(int screenWidth, Texture2D tex);
    void Update(float dt) override;
    void Draw() override;
    bool IsOffScreen(int screenWidth, int screenHeight) override;
};