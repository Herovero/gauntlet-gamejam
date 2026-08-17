#pragma once
#include "raylib.h"

class WauBulan {
    public:
        // Object properties
        Vector2 pos;
        float radius;
        float speed;

        // Sprite properties
        Texture2D texture;
        float spriteWidth;
        float spriteHeight;

        WauBulan(float startX, float startY, const char* texturePath);
        void Update(float dt, int screenWidth, int screenHeight);
        void Draw();
        void Unload();
};