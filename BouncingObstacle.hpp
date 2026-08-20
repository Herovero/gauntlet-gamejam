#pragma once
#include "Obstacle.hpp"

class BouncingObstacle : public Obstacle {
private:
    float speedX;
    float speedY;
    int screenWidth;

public:
    Vector2 pos;   
    float radius;  
    Texture2D texture;

    BouncingObstacle(int screenWidth, Texture2D tex);
    void Update(float dt) override;
    void Draw() override;
    bool IsOffScreen(int screenWidth, int screenHeight) override;
    
    bool CheckCollision(Vector2 playerPos, float playerRadius) override;
};