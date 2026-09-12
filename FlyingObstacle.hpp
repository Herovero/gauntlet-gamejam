#pragma once
#include "Obstacle.hpp"
#include <vector>

class FlyingObstacle : public Obstacle {
private:
    float speedX;
    float speedY;
    bool flyingLeft;

    //Texture2D texture;
    std::vector<Texture2D> frames;
    int totalFrames;
    int currentFrame;
    float frameTimer;
    float frameSpeed;
    int animDirection;

public:
    FlyingObstacle(int screenWidth, int screenHeight, bool flyLeft, std::vector<Texture2D> animFrames, float bgSpeed);
    void Update(float dt) override;
    void Draw() override;
    bool IsOffScreen(int screenWidth, int screenHeight) override;
    void SyncBackgroundSpeed(float bgSpeed) override;
};