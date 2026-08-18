#include "raylib.h"
#include "WauBulan.hpp"
#include "SwingingKid.hpp"
#include "Obstacle.hpp"
#include "Background.hpp"

int main() {
    // Screen Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;

    // Run the window and show title
    InitWindow(screenWidth, screenHeight, "Wau Bulan Endless Runner");

    // Framerate per second
    SetTargetFPS(60); 

    Background bg("assets/background.png", screenWidth, screenHeight, 30.0f);

    WauBulan wau(screenWidth / 2.0f, screenHeight - 600.0f, "assets/waubulan.png");
    SwingingKid kid(wau.pos);
    Obstacle obstacle(screenWidth, screenHeight);

    // Game State
    bool isGameOver = false;

    // Main Game Loop
    // WindowShouldClose() returns true if pressing escape or close buton
    while (!WindowShouldClose()) { 
        float dt = GetFrameTime();

        if (!isGameOver){
            bg.Update(dt);
            wau.Update(dt, screenWidth, screenHeight);
            kid.Update(dt, wau.pos);
            obstacle.Update(dt, screenWidth, screenHeight);

            if (CheckCollisionCircleRec(wau.pos, wau.radius, obstacle.rec) ||
                CheckCollisionCircleRec(kid.pos, kid.radius, obstacle.rec)) {
                isGameOver = true;
            }
        } else {
            if (IsKeyPressed(KEY_SPACE)) {
                isGameOver = false;
                bg.Reset();
                wau.pos = { (float)screenWidth / 2.0f, (float)screenHeight - 600.0f };
                kid = SwingingKid(wau.pos);
                obstacle.Reset(screenWidth, screenHeight);
            }
        }

        // Drawing logic
        BeginDrawing();
            // Switch background color
            ClearBackground(SKYBLUE);

            bg.Draw();

            // Example: DrawText(text, x_position, y_position, font_size, color)
            if (!isGameOver){
                wau.Draw();
                kid.Draw(wau.pos);
                obstacle.Draw();
            } else {
                DrawText("GAME OVER", screenWidth / 2 - 110, screenHeight / 2 - 30, 40, RED);
                DrawText("Press SPACE to Restart", screenWidth / 2 - 120, screenHeight / 2 + 20, 20, DARKGRAY);
            }

        EndDrawing();
    }

    wau.Unload();
    bg.Unload();
    CloseWindow(); 

    return 0;
}

