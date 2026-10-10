#pragma once
#include "raylib.h"

class AisKacang {
public:
    Vector2 pos;
    float radius;
    Texture2D texture;
    float fallSpeed;

    AisKacang(int screenWidth, Texture2D tex);
    void Update(float dt);
    void Draw();
    bool CheckCollision(Vector2 playerPos, float playerRadius);
};