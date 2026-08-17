#include "SwingingKid.hpp"
#include <cmath>

SwingingKid::SwingingKid(Vector2 anchorPos) {
    pos = { anchorPos.x, anchorPos.y + 120.0f };
    velocity = { 0.0f, 0.0f };
    radius = 15.0f;
    stringLength = 120.0f;
    gravity = 1200.0f;
}

void SwingingKid::Update(float dt, Vector2 anchorPos) {
    // Add gravity
    velocity.y += gravity * dt;

    // Add air resistance to avoid swinging forver
    velocity.x *= 0.99f;
    velocity.y *= 0.99f;

    // Move based on velocity
    pos.x += velocity.x * dt;
    pos.y += velocity.y * dt;

    // String constraint
    float dx = pos.x - anchorPos.x;
    float dy = pos.y - anchorPos.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    // If the player gets further than the string pull them back
    if (distance > stringLength) {
        // Find the angle/direction of the string
        float dirX = dx / distance;
        float dirY = dy / distance;
        
        // Snap the position to the edge
        pos.x = anchorPos.x + (dirX * stringLength);
        pos.y = anchorPos.y + (dirY * stringLength);
        
        // Adjust velocity to flow along the arc
        float dotProduct = (velocity.x * dirX) + (velocity.y * dirY);
        velocity.x -= dotProduct * dirX;
        velocity.y -= dotProduct * dirY;
    }
}

void SwingingKid::Draw(Vector2 anchorPos) {
    DrawLineEx(anchorPos, pos, 2.0f, RAYWHITE);
    DrawCircleV(pos, radius, ORANGE);
}