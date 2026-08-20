#include "FallingObstacle.hpp"

FallingObstacle::FallingObstacle(int screenWidth, Texture2D tex) {
    texture = tex;

    float renderWidth = 50.0f;
    float aspectRatio = (float)texture.height / (float)texture.width;
    float renderHeight = renderWidth * aspectRatio;

    radius = renderWidth / 2.0f; 

    pos.x = (float)GetRandomValue((int)radius, screenWidth - (int)radius);
    pos.y = -renderHeight / 2.0f;
    speedY = (float)GetRandomValue(180, 320);
}

void FallingObstacle::Update(float dt) {
    pos.y += speedY * dt;
}

void FallingObstacle::Draw() {
    if (texture.id > 0) {
        float renderWidth = radius * 2.0f;
        float aspectRatio = (float)texture.height / (float)texture.width;
        float renderHeight = renderWidth * aspectRatio;

        Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
        Rectangle dest   = { pos.x, pos.y, renderWidth, renderHeight };
        Vector2 origin   = { renderWidth / 2.0f, renderHeight / 2.0f };

        DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
    } else {
        DrawCircleV(pos, radius, RED);
    }
}

bool FallingObstacle::IsOffScreen(int /*screenWidth*/, int screenHeight) {
    float renderHeight = (radius * 2.0f) * ((float)texture.height / (float)texture.width);
    return (pos.y - renderHeight / 2.0f) > (float)screenHeight;
}

bool FallingObstacle::CheckCollision(Vector2 playerPos, float playerRadius) {
    return CheckCollisionCircles(pos, radius, playerPos, playerRadius);
}