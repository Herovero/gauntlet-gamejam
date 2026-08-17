#include "raylib.h"
#include <cmath>

int main() {
    // Screen Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;

    // Run the window and show title
    InitWindow(screenWidth, screenHeight, "Wau Bulan Endless Runner");

    // Framerate per second
    SetTargetFPS(60); 

    // Initialize wau object
    Vector2 wauPos = {(float) screenWidth / 2.0f, (float) screenHeight - 200.0f}; // Position
    float wauRadius = 25.0f;                                                           // Size
    float moveSpeed = 250.f;

    // Initialize obstacle object
    float obsWidth = 150.0f;
    float obsHeight = 30.0f;

    // Initialize player object
    Vector2 playerPos = { wauPos.x, wauPos.y + 120.0f };
    Vector2 playerVelocity = { 0.0f, 0.0f };
    float stringLength = 120.0f;
    float playerRadius = 15.0f;

    // Randomize obstacle x position
    float randomStartX = (float)GetRandomValue(0, screenWidth - (int)obsWidth);

    // Rectangle format: { x, y, width, height }
    Rectangle obstacle = {randomStartX, -50.0f, obsWidth, obsHeight};
    float fallSpeed = 150.0f;

    // Game State
    bool isGameOver = false;

    // Main Game Loop
    // WindowShouldClose() returns true if pressing escape or close buton
    while (!WindowShouldClose()) { 
        float deltaTime = GetFrameTime();

        // Player controls
        if (!isGameOver){
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)){
                wauPos.x -= moveSpeed * deltaTime;
            }
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)){
                wauPos.x += moveSpeed * deltaTime;
            }
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)){
                wauPos.y -= moveSpeed * deltaTime;
            }
            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){
                wauPos.y += moveSpeed * deltaTime;
            }
        } else {
            if (IsKeyPressed(KEY_SPACE)) {
                isGameOver = false;
                obstacle.y = -50.0f;
                obstacle.x = (float)GetRandomValue(0, screenWidth - (int)obsWidth);
                wauPos.x = (float)screenWidth / 2.0f;
            }
        }
        
        // Add negative x boundary
        if (wauPos.x < wauRadius){
            wauPos.x = wauRadius;
        }

        // Add positive x boundary
        if (wauPos.x > (float)screenWidth - wauRadius){
            wauPos.x = (float)screenWidth - wauRadius;
        }

        // Move the obstacle down
        obstacle.y += fallSpeed * deltaTime;
        
        // After obstacle reaches bottom, go back to the top and randomize x position
        if (obstacle.y > (float)screenHeight) {
            obstacle.y = -50.0f;
            obstacle.x = (float)GetRandomValue(0, screenWidth - (int)obsWidth);
        }

        // Detect collision
        if (CheckCollisionCircleRec(wauPos, wauRadius, obstacle)) {
            isGameOver = true;
        }

        // Swinging physics
        float gravity = 1200.0f; 
        playerVelocity.y += gravity * deltaTime;

        // Add air resistance to avoid swinging forver
        playerVelocity.x *= 0.99f;
        playerVelocity.y *= 0.99f;

        // Move the player based on velocity
        playerPos.x += playerVelocity.x * deltaTime;
        playerPos.y += playerVelocity.y * deltaTime;

        // string constraint
        float dx = playerPos.x - wauPos.x;
        float dy = playerPos.y - wauPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        // If the player gets further than the string pull them back
        if (distance > stringLength) {
            // Find the angle/direction of the string
            float dirX = dx / distance;
            float dirY = dy / distance;
                
            // Snap the player position to the edge
            playerPos.x = wauPos.x + (dirX * stringLength);
            playerPos.y = wauPos.y + (dirY * stringLength);
                
            // Adjust velocity to flow along the arc
            float dotProduct = (playerVelocity.x * dirX) + (playerVelocity.y * dirY);
            playerVelocity.x -= dotProduct * dirX;
            playerVelocity.y -= dotProduct * dirY;
        }

        // Drawing logic
        BeginDrawing();
            // Switch background color
            ClearBackground(SKYBLUE);

            // Example: DrawText(text, x_position, y_position, font_size, color)
            if (!isGameOver){
                DrawRectangleRec(obstacle, RED);

                // Add wind sway to the visual
                float windSway = std::sin(GetTime() * 4.0f) * 6.0f;
                Vector2 renderPos = { wauPos.x + windSway, wauPos.y };

                DrawCircleV(renderPos, wauRadius, BLACK);
                DrawCircleV(playerPos, playerRadius, ORANGE);
            } else {
                DrawText("GAME OVER", screenWidth / 2 - 110, screenHeight / 2 - 30, 40, RED);
                DrawText("Press SPACE to Restart", screenWidth / 2 - 120, screenHeight / 2 + 20, 20, DARKGRAY);
            }

        EndDrawing();
    }

    // Safely free resources and close the window
    CloseWindow(); 

    return 0;
}