#pragma once
#include "raylib.h"

class WauBulan {
    public:
        // Object & Physics properties
        Vector2 pos;
        float radius;
        float speed;

        // Sprite properties
        Texture2D texture;
        float spriteWidth;
        float spriteHeight;

        // Visual Polish properties
        float rotation;         // Current
        float targetRotation;   // Desired

        WauBulan(float startX, float startY, const char* texturePath);
        void Update(float dt, int screenWidth, int screenHeight);
        void Draw();
        void Unload();
};