#include "Coin.hpp"
#include <cmath>

Coin::Coin(int screenWidth, Texture2D tex) {
    texture = tex;
    radius = 25.0f;
    speedY = 50.0f;
    active = true;
    
    pos.x = (float)GetRandomValue(100, screenWidth - 100);
    pos.y = -50.0f;
}

void Coin::Update(float dt) {
    if (active) pos.y += speedY * dt;
}

/*void Coin::Draw() {
    if (!active) return;

    if (texture.id > 0) {
        float baseWidth = radius * 3.0f;
        float aspectRatio = (float)texture.height / (float)texture.width;
        float renderHeight = baseWidth * aspectRatio;

        // Spin logic
        float time = (float)GetTime();
        float spinSpeed = 6.0f; // Increase this to spin faster
        
        // Use cos() to oscillate between -1.0 and 1.0. 
        // Adding pos.x creates a unique offset for every coin
        float spinFactor = std::cos(time * spinSpeed + pos.x * 0.01f); 

        // If the coin is spinning backward, flip the source texture
        float srcWidth = (spinFactor < 0.0f) ? -(float)texture.width : (float)texture.width;
        Rectangle source = { 0.0f, 0.0f, srcWidth, (float)texture.height };

        // Use absolute value (fabs) for the destination width so it scales from the center safely
        float currentWidth = baseWidth * std::fabs(spinFactor);
        
        Rectangle dest = { pos.x, pos.y, currentWidth, renderHeight };
        Vector2 origin = { currentWidth / 2.0f, renderHeight / 2.0f };

        DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
    } else {
        DrawCircleV(pos, radius, MAGENTA); 
    }
}*/

void Coin::Draw() {
    if (!active) return;

    if (texture.id > 0) {
        // Force the render size to perfectly match the 64x64 PNG
        // (Change to texture.width * 2.0f if you want it twice as big!)
        float baseWidth = (float)texture.width * 2.0f;
        float renderHeight = (float)texture.height * 2.0f;

        // Spin logic
        float time = (float)GetTime();
        float spinSpeed = 6.0f; // Increase this to spin faster

        // Use cos() to oscillate between -1.0 and 1.0. 
        // Adding pos.x creates a unique offset for every coin
        float spinFactor = std::cos(time * spinSpeed + pos.x * 0.01f);

        // If the coin is spinning backward, flip the source texture
        //float srcWidth = (spinFactor < 0.0f) ? -(float)texture.width : (float)texture.width;
        //Rectangle source = { 0.0f, 0.0f, srcWidth, (float)texture.height };

        Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };

        // Use absolute value (fabs) for the destination width so it scales from the center safely
        float currentWidth = baseWidth * std::fabs(spinFactor);
        
        // Pixel snapping applied to the destination
        Rectangle dest   = { (float)((int)pos.x), (float)((int)pos.y), currentWidth, renderHeight };
        Vector2 origin   = { currentWidth / 2.0f, renderHeight / 2.0f };

        DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
    } else {
        DrawCircleV(pos, radius, MAGENTA); 
    }
}

bool Coin::CheckCollision(Vector2 playerPos, float playerRadius) {
    if (!active) return false;
    return CheckCollisionCircles(pos, radius, playerPos, playerRadius);
}