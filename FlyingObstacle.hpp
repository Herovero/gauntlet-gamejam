#pragma once
#include "Obstacle.hpp"

class FlyingObstacle : public Obstacle {
private:
    float speedX;
    float speedY;
    bool flyingLeft;
    Texture2D texture;
public:
    FlyingObstacle(int screenWidth, int screenHeight, bool flyLeft, Texture2D texture, float bgSpeed);
    void Update(float dt) override;
    void Draw() override;
    bool IsOffScreen(int screenWidth, int screenHeight) override;
    void SyncBackgroundSpeed(float bgSpeed) override;
};