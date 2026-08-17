#pragma once
#include "raylib.h"

class WauBulan {
    public:
        float pos;
        float radius;
        float speed;

        WauBulan(float startX, startY);
        void Update(float dt, int screenWidth, int screenHeight);
        void Draw();
}