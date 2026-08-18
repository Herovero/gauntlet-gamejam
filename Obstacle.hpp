#pragma once
#include "raylib.h"

enum ObstacleType {
    TYPE_FALLING,
    TYPE_FLYING_LEFT,
    TYPE_FLYING_RIGHT 
};

class Obstacle {
public:
    Rectangle rec;
    float speedX;
    float speedY;
    ObstacleType type;

    Obstacle(int screenWidth, int screenHeight); 
    void Update(float dt, int screenWidth, int screenHeight);
    void Reset(int screenWidth, int screenHeight);
    void Draw();
};