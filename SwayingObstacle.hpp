#pragma once
#include "Obstacle.hpp"

class SwayingObstacle : public Obstacle {
public:
    float speedY, baseX, swayOffset;
    Texture2D texture;

    SwayingObstacle(int screenWidth, Texture2D tex);
    void Update(float dt) override;
    void Draw() override;
    bool IsOffScreen(int screenWidth, int screenHeight) override;
};