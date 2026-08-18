#include "Obstacle.hpp"

Obstacle::Obstacle(int screenWidth, int screenHeight) {
    Reset(screenWidth, screenHeight);
}

void Obstacle::Update(float dt, int screenWidth, int screenHeight) {
    rec.x += speedX * dt;
    rec.y += speedY * dt;

    if (type == TYPE_FALLING && rec.y > (float)screenHeight) {
        Reset(screenWidth, screenHeight);
    }
    else if (type == TYPE_FLYING_LEFT && rec.x + rec.width < 0.0f) {
        Reset(screenWidth, screenHeight);
    }
    else if (type == TYPE_FLYING_RIGHT && rec.x > (float)screenWidth) {
        Reset(screenWidth, screenHeight);
    }
}

void Obstacle::Reset(int screenWidth, int screenHeight) {
    int randomType = GetRandomValue(0, 2);
    
    // After obstacle reaches bottom, go back to the top and randomize x position
    if (randomType == 0) {
        type = TYPE_FALLING;
        rec.width = 150.0f;
        rec.height = 30.0f;
        rec.x = (float)GetRandomValue(0, screenWidth - (int)rec.width);
        rec.y = -50.0f;      // Start above screen
        speedX = 0.0f;       
        speedY = 150.0f;     // Falling speed
    }
    // After obstacle reaches left, go back to right and randomize y position
    else if (randomType == 1) {
        type = TYPE_FLYING_LEFT;
        rec.width = 60.0f;
        rec.height = 30.0f;
        rec.x = (float)screenWidth + 50.0f;
        rec.y = (float)GetRandomValue(50, screenHeight / 2);
        speedX = -250.0f;    
        speedY = 0.0f;
    }
    // After obstacle reaches right, go back to left and randomize y position
    else if (randomType == 2) {
        type = TYPE_FLYING_RIGHT;
        rec.width = 60.0f;
        rec.height = 30.0f;
        rec.x = -50.0f;
        rec.y = (float)GetRandomValue(50, screenHeight / 2);
        speedX = 250.0f;    
        speedY = 0.0f;
    }
}

void Obstacle::Draw() {
    if (type == TYPE_FALLING) {
        DrawRectangleRec(rec, RED);  
    } else {
        DrawRectangleRec(rec, ORANGE); 
    }
}