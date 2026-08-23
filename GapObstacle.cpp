#include "GapObstacle.hpp"

GapObstacle::GapObstacle(int screenWidth, Texture2D tex) {
    texture = tex;
    // Fixed horizontal width of gap
    float gapWidth = 350.0f;
    
    // Pick a random X coordinate for where the gap starts
    float gapX = (float)GetRandomValue(50, screenWidth - (int)gapWidth - 50);

    rec.x = 0.0f; // Spawn the first beam from left screen border
    rec.y = -50.0f; // Spawn the beam from above
    rec.width = gapX; // The end width of left beam is the start of the gap
    rec.height = 80.0f;

    rec2.x = gapX + gapWidth; // Spawn the second beam after the end of the gap
    rec2.y = -50.0f; // Spawn the beam from above
    rec2.width = (float)screenWidth - rec2.x; // Extend the right beam's width until it reaches the right border
    rec2.height = 80.0f; 
    
    speedY = 120;
}

void GapObstacle::Update(float dt) {
    rec.y += speedY * dt;
    rec2.y += speedY * dt;
}

void GapObstacle::Draw() {
    if (texture.id > 0) {
        // Define the source rectangle using the entire original image dimensions
        Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
        // rec = screen position
        // { 0, 0 } = pivot point for rotation/scaling
        // 0.0f = rotation angle
        DrawTexturePro(texture, source, rec, { 0, 0 }, 0.0f, WHITE);
        DrawTexturePro(texture, source, rec2, { 0, 0 }, 0.0f, WHITE);
    } else {
        DrawRectangleRec(rec, DARKGRAY);
        DrawRectangleRec(rec2, DARKGRAY);
    }
}

bool GapObstacle::IsOffScreen(int /*screenWidth*/, int screenHeight) { return rec.y > screenHeight; }

bool GapObstacle::CheckCollision(Vector2 pos, float radius) {
    return CheckCollisionCircleRec(pos, radius, rec) || CheckCollisionCircleRec(pos, radius, rec2);
}