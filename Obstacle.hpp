#pragma once
#include "raylib.h"

class Obstacle {
public:
    Rectangle rec;
    
    virtual ~Obstacle() = default; 

    virtual void Update(float dt) = 0;
    virtual void Draw() = 0;
    virtual bool IsOffScreen(int screenWidth, int screenHeight) = 0;
    
    virtual bool CheckCollision(Vector2 pos, float radius) {
        return CheckCollisionCircleRec(pos, radius, rec);
    }
    
    virtual bool IsGapType() { return false; }
};