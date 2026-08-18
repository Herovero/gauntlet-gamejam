#include "raylib.h"
#include "WauBulan.hpp"
#include "SwingingKid.hpp"
#include "Obstacle.hpp"
#include "Background.hpp"
#include "ObstacleSpawner.hpp"
#include "CollisionManager.hpp"
#include "ScoreManager.hpp"

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
    SwingingKid kid(wau.pos, "assets/kid.png", "assets/kid_falling.png");
    ObstacleSpawner spawner(screenWidth, screenHeight);
    ScoreManager scoreManager;

    // Game State
    bool isGameOver = false;

    // Main Game Loop
    // WindowShouldClose() returns true if pressing escape or close buton
    while (!WindowShouldClose()) { 
        float dt = GetFrameTime();

        if (!isGameOver){
            bg.Update(dt);

            if (!kid.isDetached) {
                wau.Update(dt, screenWidth, screenHeight);
            } else {
                wau.pos.y += 400.0f * dt; 
            }

            kid.Update(dt, wau.pos);
            spawner.Update(dt);
            scoreManager.Update(dt, kid.isDetached);

            // Check collisions only if the string is still attached
            if (!kid.isDetached && CollisionManager::CheckPlayerCollisions(wau, kid, spawner)) {
                kid.isDetached = true;
                
                // Give the kid a little visual bump when hitting an obstacle to emphasize the impact
                kid.velocity.y = -300.0f; 
                kid.velocity.x = -150.0f; 
            }

            // Trigger the game over screen when the kid drops out of view
            if (kid.isDetached && (kid.pos.y - kid.radius) > (float)screenHeight) {
                isGameOver = true;
            }
        } else {
            if (IsKeyPressed(KEY_SPACE)) {
                isGameOver = false;
                bg.Reset();
                wau.pos = { (float)screenWidth / 2.0f, (float)screenHeight - 600.0f };

                kid.pos = { wau.pos.x, wau.pos.y + 120.0f };
                kid.velocity = { 0.0f, 0.0f };
                kid.isDetached = false;

                spawner.Reset();
                scoreManager.Reset();
            }
        }

        // Drawing logic
        BeginDrawing();
            // Switch background color
            ClearBackground(SKYBLUE);

            bg.Draw();

            if (!isGameOver){
                wau.Draw();
                kid.Draw(wau.pos);
                spawner.Draw();
                scoreManager.Draw();
            } else {
                scoreManager.DrawGameOver(screenWidth, screenHeight);
            }

        EndDrawing();
    }

    wau.Unload();
    kid.Unload();
    bg.Unload();
    spawner.Unload();
    CloseWindow(); 

    return 0;
}

