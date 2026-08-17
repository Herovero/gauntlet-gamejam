#include "raylib.h"
#include <cmath>

int main() {
    // Screen Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;

    // Run the window and show title
    InitWindow(screenWidth, screenHeight, "Game Jam - Hello World");

    // Framerate per second
    SetTargetFPS(60); 

    // Initialize player object
    Vector2 circlePosition = {(float) screenWidth / 2.0f, (float) screenHeight - 100.0f}; // Position
    float circleRadius = 25.0f;                                                           // Size
    float moveSpeed = 250.f;

    // Initialize obstacle object
    float obsWidth = 150.0f;
    float obsHeight = 30.0f;

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
                circlePosition.x -= moveSpeed * deltaTime;
            }
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)){
                circlePosition.x += moveSpeed * deltaTime;
            }
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)){
                circlePosition.y -= moveSpeed * deltaTime;
            }
            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){
                circlePosition.y += moveSpeed * deltaTime;
            }
        } else {
            if (IsKeyPressed(KEY_SPACE)) {
                isGameOver = false;
                obstacle.y = -50.0f;
                obstacle.x = (float)GetRandomValue(0, screenWidth - (int)obsWidth);
                circlePosition.x = (float)screenWidth / 2.0f;
            }
        }
        
        // Add negative x boundary
        if (circlePosition.x < circleRadius){
            circlePosition.x = circleRadius;
        }

        // Add positive x boundary
        if (circlePosition.x > (float)screenWidth - circleRadius){
            circlePosition.x = (float)screenWidth - circleRadius;
        }

        // Move the obstacle down
        obstacle.y += fallSpeed * deltaTime;
        
        // After obstacle reaches bottom, go back to the top and randomize x position
        if (obstacle.y > (float)screenHeight) {
            obstacle.y = -50.0f;
            obstacle.x = (float)GetRandomValue(0, screenWidth - (int)obsWidth);
        }

        // Detect collision
        if (CheckCollisionCircleRec(circlePosition, circleRadius, obstacle)) {
            isGameOver = true;
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
                Vector2 renderPos = { circlePosition.x + windSway, circlePosition.y };

                DrawCircleV(renderPos, circleRadius, BLACK);
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