#include "raylib.h"

int main() {
    // Screen Initialization
    const int screenWidth = 450;
    const int screenHeight = 800;

    // Run the window and show title
    InitWindow(screenWidth, screenHeight, "Game Jam - Hello World");

    // Framerate per second
    SetTargetFPS(60); 

    // Initialize the object
    Vector2 circlePosition = {(float) screenWidth / 2.0f, (float) screenHeight - 100.0f}; // Position
    float circleRadius = 25.0f;                                                           // Size
    float risingSpeed = 50.f;
    float moveSpeed = 250.f;

    // Main Game Loop
    // WindowShouldClose() returns true if pressing escape or close buton
    while (!WindowShouldClose()) { 
        float deltaTime = GetFrameTime();

        // Player controls
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)){
            circlePosition.x -= moveSpeed * deltaTime;
        }
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)){
            circlePosition.x += moveSpeed * deltaTime;
        }
        
        // Add negative x boundary
        if (circlePosition.x < circleRadius){
            circlePosition.x = circleRadius;
        }

        // Add positive x boundary
        if (circlePosition.x > (float)screenWidth - circleRadius){
            circlePosition.x = (float)screenWidth - circleRadius;
        }

        // Move the object above
        circlePosition.y -= risingSpeed * deltaTime;

        // Object reset spawning from below after flying off screen above
        if (circlePosition.y < -circleRadius) {
            circlePosition.y = screenHeight + circleRadius;
        }

        // Drawing logic
        BeginDrawing();
            // Always clear background first to avoid graphical issues
            ClearBackground(RAYWHITE);

            // Example: DrawText(text, x_position, y_position, font_size, color)
            DrawCircleV(circlePosition, circleRadius, BLACK);

        EndDrawing();
    }

    // Safely free resources and close the window
    CloseWindow(); 

    return 0;
}