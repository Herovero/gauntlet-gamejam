#pragma once
#include "raylib.h"

enum ObstacleType {
    TYPE_FALLING,
    TYPE_FLYING_LEFT,
    TYPE_FLYING_RIGHT,
    TYPE_SWAYING,
    TYPE_GAP
};

class Obstacle {
public:
    Rectangle rec;
    Rectangle rec2;
    float speedX;
    float speedY;
    ObstacleType type;

    float baseX;      
    float swayOffset;

    Obstacle(int screenWidth, int screenHeight, bool allowGap); 
    void Update(float dt, int screenWidth, int screenHeight, bool allowGap);
    void Reset(int screenWidth, int screenHeight, bool allowGap);
    void Draw();
};