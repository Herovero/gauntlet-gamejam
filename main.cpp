#include "raylib.h"

int main() {
    // Screen Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    // Run the window and show title
    InitWindow(screenWidth, screenHeight, "Game Jam - Hello World");

    // Framerate per second
    SetTargetFPS(60); 

    // Main Game Loop
    // WindowShouldClose() returns true if pressing escape or close buton
    while (!WindowShouldClose()) { 
        // Update logic (example: Player movement, collision detection)
        
        // Drawing logic
        BeginDrawing();
            // Always clear background first to avoid graphical issues
            ClearBackground(RAYWHITE);

            // DrawText(text, x_position, y_position, font_size, color)
            DrawText("Hello World!", 300, 200, 30, DARKGRAY);
            
        EndDrawing();
    }

    // Safely free resources and close the window
    CloseWindow(); 

    return 0;
}