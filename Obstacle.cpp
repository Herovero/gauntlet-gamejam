#include "Obstacle.hpp"
#include <cmath>

Obstacle::Obstacle(int screenWidth, int screenHeight, bool allowGap) {
    Reset(screenWidth, screenHeight, allowGap);
}

void Obstacle::Update(float dt, int screenWidth, int screenHeight, bool allowGap) {
    if (type == TYPE_SWAYING) {
        // Move down slowly
        rec.y += speedY * dt;

        // Sways left and right around its baseX using time and sine
        float time = (float)GetTime();
        float frequency = 3.0f;   // Sway speed
        float amplitude = 120.0f; // Maximum distance swayed from center

        rec.x = baseX + std::sin(time * frequency + swayOffset) * amplitude;
    } 
    else if (type == TYPE_GAP) {
        // Move BOTH walls downwards
        rec.y += speedY * dt;
        rec2.y += speedY * dt;
    }
    else {
        rec.x += speedX * dt;
        rec.y += speedY * dt;
    }

    if ((type == TYPE_FALLING || type == TYPE_SWAYING || type == TYPE_GAP) && rec.y > (float)screenHeight) {
        Reset(screenWidth, screenHeight, allowGap);
    }
    else if (type == TYPE_FLYING_LEFT && rec.x + rec.width < 0.0f) {
        Reset(screenWidth, screenHeight, allowGap);
    }
    else if (type == TYPE_FLYING_RIGHT && rec.x > (float)screenWidth) {
        Reset(screenWidth, screenHeight, allowGap);
    }
}

void Obstacle::Reset(int screenWidth, int screenHeight, bool allowGap) {
    // Restrict the random pool if a gap already exists
    int maxRandom = allowGap ? 4 : 3;
    int randomType = GetRandomValue(0, maxRandom);
    
    // After obstacle reaches bottom, go back to the top and randomize x position
    if (randomType == 0) {
        type = TYPE_FALLING;
        rec.width = 150.0f;
        rec.height = 30.0f;
        rec.x = (float)GetRandomValue(0, screenWidth - (int)rec.width);
        rec.y = -50.0f;      // Start from above
        speedX = 0.0f;       
        speedY = 150.0f;     // Falling speed
    }
    // After obstacle reaches left, go back to right and randomize y position
    else if (randomType == 1) {
        type = TYPE_FLYING_LEFT;
        rec.width = 60.0f;
        rec.height = 30.0f;
        rec.x = (float)screenWidth + 50.0f;  // Start from right
        rec.y = (float)GetRandomValue(50, screenHeight / 2);
        speedX = -250.0f;    // Flying left speed
        speedY = 0.0f;
    }
    // After obstacle reaches right, go back to left and randomize y position
    else if (randomType == 2) {
        type = TYPE_FLYING_RIGHT;
        rec.width = 60.0f;
        rec.height = 30.0f;
        rec.x = -50.0f;     // Start from right
        rec.y = (float)GetRandomValue(50, screenHeight / 2);
        speedX = 250.0f;    // Flying right speed
        speedY = 0.0f;
    }
    else if (randomType == 3) {
        type = TYPE_SWAYING;
        rec.width = 45.0f; 
        rec.height = 45.0f;
        
        // Pick a center point that leaves room for the 120px sway radius
        // Make object sway around 120px radius
        baseX = (float)GetRandomValue(120, screenWidth - 120);
        rec.x = baseX;
        rec.y = -50.0f;    // Start from above
        
        speedX = 0.0f;
        speedY = 100.0f;
        
        // Randomize sway
        swayOffset = (float)GetRandomValue(0, 100);
    }
    else if (randomType == 4) {
        type = TYPE_GAP;
        
        // Define the size of the safe zone
        float gapWidth = 250.0f; 
        
        // Randomize where the gap starts
        float gapX = (float)GetRandomValue(50, screenWidth - (int)gapWidth - 50);

        // Build the left rec
        rec.x = 0.0f;
        rec.y = -50.0f;
        rec.width = gapX;
        rec.height = 40.0f;

        // Build the right rec
        rec2.x = gapX + gapWidth;
        rec2.y = -50.0f;
        rec2.width = (float)screenWidth - rec2.x;
        rec2.height = 40.0f;

        speedX = 0.0f;
        speedY = 125.0f;
    }
}

void Obstacle::Draw() {
    if (type == TYPE_FALLING) {
        DrawRectangleRec(rec, RED);
    }
    else if (type == TYPE_SWAYING) {
        DrawRectangleRec(rec, PURPLE);
    } 
    else if (type == TYPE_GAP) {
        DrawRectangleRec(rec, GRAY);
        DrawRectangleRec(rec2, GRAY);
    }
    else {
        DrawRectangleRec(rec, ORANGE); 
    }
}