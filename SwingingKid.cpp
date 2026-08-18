#include "SwingingKid.hpp"
#include <cmath>

SwingingKid::SwingingKid(Vector2 anchorPos, const char* imagePath) {
    pos = { anchorPos.x, anchorPos.y + 120.0f };
    velocity = { 0.0f, 0.0f };
    radius = 20.0f;
    stringLength = 500.0f;
    gravity = 1200.0f;

    texture = LoadTexture(imagePath);
}

void SwingingKid::Update(float dt, Vector2 anchorPos) {
    // Add gravity
    velocity.y += gravity * dt;

    // Add air resistance to avoid swinging forever
    velocity.x *= 0.99f;
    velocity.y *= 0.99f;

    // Move based on velocity
    pos.x += velocity.x * dt;
    pos.y += velocity.y * dt;

    // String constraint
    float dx = pos.x - anchorPos.x;
    float dy = pos.y - anchorPos.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    // If the player gets further than the string, pull them back
    if (distance > stringLength) {
        float dirX = dx / distance;
        float dirY = dy / distance;
        
        pos.x = anchorPos.x + (dirX * stringLength);
        pos.y = anchorPos.y + (dirY * stringLength);
        
        float dotProduct = (velocity.x * dirX) + (velocity.y * dirY);
        velocity.x -= dotProduct * dirX;
        velocity.y -= dotProduct * dirY;
    }
}

void SwingingKid::Draw(Vector2 anchorPos) {
    // Draw string connecting kite to the kid's center
    DrawLineEx(anchorPos, pos, 2.0f, RAYWHITE);

    if (texture.id > 0 && texture.width > 0) {
        float renderWidth = radius * 2.5f;
        float aspectRatio = (float)texture.height / (float)texture.width;
        float renderHeight = renderWidth * aspectRatio;

        Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
        Rectangle dest   = { pos.x, pos.y, renderWidth, renderHeight };
        Vector2 origin   = { renderWidth / 2.0f, renderHeight / 2.0f };

        DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
    } else {
        DrawCircleV(pos, radius, ORANGE);
    }

    // Collision Hitbox Debug
    DrawCircleLines((int)pos.x, (int)pos.y, radius, GREEN);
}

void SwingingKid::Unload() {
    if (texture.id > 0) {
        UnloadTexture(texture);
    }
}