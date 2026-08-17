#include "raylib.h"
#include <cmath>

class WauBulan {
public:
    float pos;
    float radius;
    float speed;

    WauBulan(float startX, float startY) {
        pos = { startX, startY };
        radius = 25.0f;
        speed = 250.0f;
    }

    void Update(float dt, int screenWidth, int screenHeight) {
        // Movement
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) pos.x -= speed * dt;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) pos.x += speed * dt;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) pos.y -= speed * dt;
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) pos.y += speed * dt;
    
        // Boundaries
        if (pos.x < radius) pos.x = radius;
        if (pos.x > screenWidth - radius) pos.x = screenWidth - radius;
        
        // Keeping it strictly bounded vertically as well
        if (pos.y < radius) pos.y = radius;
        if (pos.y > screenHeight - radius) pos.y = screenHeight - radius;
    }

    void Draw() {
        float windSway = std::sin(GetTime() * 4.0f) * 6.0f;
        Vector2 renderPos = { pos.x + windSway, pos.y };
        DrawCircleV(renderPos, radius, BLACK);
    }
}