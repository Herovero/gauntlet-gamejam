#pragma once
#include "raylib.h"

class Obstacle {
    public:
        Rectangle rec;
        float fallSpeed;

        Obstacle(int screenWidth);
        void Update(float dt, int screenWidth, int screenHeight);
        void Reset(int screenWidth);
        void Draw();
};