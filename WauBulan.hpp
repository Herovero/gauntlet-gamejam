#pragma once
#include "raylib.h"

class WauBulan {
    public:
        Vector2 pos;
        float radius;
        float speed;

        WauBulan(float startX, float startY);
        void Update(float dt, int screenWidth, int screenHeight);
        void Draw();
};