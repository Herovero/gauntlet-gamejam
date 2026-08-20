#include "BouncingObstacle.hpp"

BouncingObstacle::BouncingObstacle(int screenWidth, Texture2D tex) {
    texture = tex;
    radius = 22.0f;
    this->screenWidth = screenWidth;
    
    // Spawn randomly at the top
    pos.x = (float)GetRandomValue(100, screenWidth - 100);
    pos.y = -50.0f; 
    
    // Drop speed
    speedY = (float)GetRandomValue(150, 250);
    
    // Randomize initial horizontal bounce speed and direction
    speedX = (float)GetRandomValue(200, 400);
    if (GetRandomValue(0, 1) == 0) {
        speedX = -speedX;
    }
}

void BouncingObstacle::Update(float dt) {
    pos.x += speedX * dt;
    pos.y += speedY * dt;

    // Ricochet off the left and right screen boundaries
    if (pos.x - radius < 0) {
        pos.x = radius;       // Snap back to edge
        speedX = -speedX;     // Reverse horizontal direction
    } 
    else if (pos.x + radius > screenWidth) {
        pos.x = screenWidth - radius;
        speedX = -speedX;
    }
}

void BouncingObstacle::Draw() {
    if (texture.id > 0) {
        float renderWidth = radius * 2.5f;
        float aspectRatio = (float)texture.height / (float)texture.width;
        float renderHeight = renderWidth * aspectRatio;

        Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
        
        // flip the texture horizontally depending on which way it's bouncing
        if (speedX < 0) {
            source.width = -source.width; 
        }

        Rectangle dest = { pos.x, pos.y, renderWidth, renderHeight };
        Vector2 origin = { renderWidth / 2.0f, renderHeight / 2.0f };

        DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
    } else {
        DrawCircleV(pos, radius, PURPLE);
    }
}

bool BouncingObstacle::IsOffScreen(int /*screenWidth*/, int screenHeight) {
    return pos.y - radius > screenHeight;
}

bool BouncingObstacle::CheckCollision(Vector2 playerPos, float playerRadius) {
    return CheckCollisionCircles(playerPos, playerRadius, pos, radius);
}