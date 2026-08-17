#include "raylib.h"
#include "WauBulan.hpp"
#include "SwingingKid.hpp"

int main() {
    // Screen Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;

    // Run the window and show title
    InitWindow(screenWidth, screenHeight, "Wau Bulan Endless Runner");

    // Framerate per second
    SetTargetFPS(60); 

    WauBulan wau(screenWidth / 2.0f, screenHeight - 200.0f);
    SwingingKid kid(wau.pos);

    // Initialize obstacle object
    //float obsWidth = 150.0f;
    //float obsHeight = 30.0f;

    // Randomize obstacle x position
    //float randomStartX = (float)GetRandomValue(0, screenWidth - (int)obsWidth);

    // Rectangle format: { x, y, width, height }
    //Rectangle obstacle = {randomStartX, -50.0f, obsWidth, obsHeight};
    //float fallSpeed = 150.0f;

    // Game State
    bool isGameOver = false;

    // Main Game Loop
    // WindowShouldClose() returns true if pressing escape or close buton
    while (!WindowShouldClose()) { 
        float dt = GetFrameTime();

        // Player controls
        if (!isGameOver){
            wau.Update(dt, screenWidth, screenHeight);
            kid.Update(dt, wau.pos);
        } else {
            if (IsKeyPressed(KEY_SPACE)) {
                isGameOver = false;
                //obstacle.y = -50.0f;
                //obstacle.x = (float)GetRandomValue(0, screenWidth - (int)obsWidth);
            }
        }

        // Move the obstacle down
        //obstacle.y += fallSpeed * deltaTime;
        
        // After obstacle reaches bottom, go back to the top and randomize x position
        //if (obstacle.y > (float)screenHeight) {
            //obstacle.y = -50.0f;
           // obstacle.x = (float)GetRandomValue(0, screenWidth - (int)obsWidth);
        //}

        // Detect collision
        //if (CheckCollisionCircleRec(wauPos, wauRadius, obstacle)) {
            //isGameOver = true;
        //}

        // Swinging physics
        //float gravity = 1200.0f; 
        //playerVelocity.y += gravity * deltaTime;

        // Add air resistance to avoid swinging forver
        //playerVelocity.x *= 0.99f;
        //playerVelocity.y *= 0.99f;

        // Move the player based on velocity
        //playerPos.x += playerVelocity.x * deltaTime;
        //playerPos.y += playerVelocity.y * deltaTime;

        // string constraint
        //float dx = playerPos.x - wauPos.x;
        //float dy = playerPos.y - wauPos.y;
        //float distance = std::sqrt(dx * dx + dy * dy);

        // If the player gets further than the string pull them back
        //if (distance > stringLength) {
            // Find the angle/direction of the string
            //float dirX = dx / distance;
            //float dirY = dy / distance;
                
            // Snap the player position to the edge
            //playerPos.x = wauPos.x + (dirX * stringLength);
            //playerPos.y = wauPos.y + (dirY * stringLength);
                
            // Adjust velocity to flow along the arc
            //float dotProduct = (playerVelocity.x * dirX) + (playerVelocity.y * dirY);
            //playerVelocity.x -= dotProduct * dirX;
            //playerVelocity.y -= dotProduct * dirY;
        //}

        // Drawing logic
        BeginDrawing();
            // Switch background color
            ClearBackground(SKYBLUE);

            // Example: DrawText(text, x_position, y_position, font_size, color)
            if (!isGameOver){
                wau.Draw();
                kid.Draw(wau.pos);
                //DrawRectangleRec(obstacle, RED);
            } else {
                DrawText("GAME OVER", screenWidth / 2 - 110, screenHeight / 2 - 30, 40, RED);
                DrawText("Press SPACE to Restart", screenWidth / 2 - 120, screenHeight / 2 + 20, 20, DARKGRAY);
            }

        EndDrawing();
    }
    CloseWindow(); 

    return 0;
}

