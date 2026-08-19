#pragma once
#include "raylib.h"

class BungaRaya {
public:
    Vector2 pos;
    float radius;
    float speedY;
    bool active;
    Texture2D texture;

    BungaRaya(int screenWidth, Texture2D tex);
    void Update(float dt);
    void Draw();
    bool CheckCollision(Vector2 playerPos, float playerRadius);
};