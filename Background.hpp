#pragma once
#include "raylib.h"

class Background {
public:
    Texture2D texture;
    float scrollY;
    float scrollSpeed;

    float drawWidth;
    float drawHeight;
    float drawX;

    int screenWidth;
    int screenHeight;

    Background(const char* imagePath, int screenWidth, int screenHeight, float scrollSpeed);
    void Update(float dt);
    void Draw();
    void Unload();
};