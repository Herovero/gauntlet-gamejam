#include "SwingingKid.hpp"
#include <cmath>

SwingingKid::SwingingKid(Vector2 anchorPos, const char* normalPath, const char* fallingPath) {
    pos = { anchorPos.x, anchorPos.y + 120.0f };
    velocity = { 0.0f, 0.0f };
    radius = 20.0f;
    stringLength = 500.0f;
    gravity = 1200.0f;
    isDetached = false;

    texture = LoadTexture(normalPath);
    texFalling = LoadTexture(fallingPath);
}

void SwingingKid::Update(float dt, Vector2 anchorPos) {
    // Add gravity
    velocity.y += gravity * dt;

    // Add air resistance to avoid swinging forever
    if (!isDetached) {
        velocity.x *= 0.99f;
        velocity.y *= 0.99f;
    }

    // Move based on velocity
    pos.x += velocity.x * dt;
    pos.y += velocity.y * dt;

    if (!isDetached) {
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
}

void SwingingKid::Draw(Vector2 anchorPos) {
    float offsetX = -15.0f;

    Vector2 visualPos = { pos.x + offsetX, pos.y };

    // Draw string connecting kite to the kid's center
    if (!isDetached) {
        DrawLineEx(anchorPos, pos, 2.0f, RAYWHITE);
    }

    // Pick active texture based on state
    Texture2D currentTex = (isDetached && texFalling.id > 0) ? texFalling : texture;

    if (currentTex.id > 0 && currentTex.width > 0) {
        float renderWidth = radius * 2.5f;
        float aspectRatio = (float)currentTex.height / (float)currentTex.width;
        float renderHeight = renderWidth * aspectRatio;

        Rectangle source = { 0.0f, 0.0f, (float)currentTex.width, (float)currentTex.height };
        Rectangle dest   = { visualPos.x, visualPos.y, renderWidth, renderHeight };
        Vector2 origin   = { renderWidth / 2.0f, renderHeight / 2.0f };

        DrawTexturePro(currentTex, source, dest, origin, 0.0f, WHITE);
    } else {
        DrawCircleV(visualPos, radius, isDetached ? RED : ORANGE);
    }

    // Collision Hitbox Debug
    //DrawCircleLines((int)visualPos.x, (int)visualPos.y, radius, GREEN);
}

void SwingingKid::Unload() {
    if (texture.id > 0) UnloadTexture(texture);
    if (texFalling.id > 0) UnloadTexture(texFalling);
}