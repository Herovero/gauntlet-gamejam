#include "raylib.h"
#include "WauBulan.hpp"
#include "SwingingKid.hpp"
#include "Obstacle.hpp"
#include "Background.hpp"
#include "ObstacleSpawner.hpp"
#include "CollisionManager.hpp"
#include "ScoreManager.hpp"
#include "ItemSpawner.hpp"

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
    ItemSpawner itemSpawner(screenWidth, screenHeight);
    ScoreManager scoreManager;

    const float NORMAL_BG_SPEED = 30.0f;
    const float BOOST_BG_SPEED = 150.0f;

    // Game State
    bool isGameOver = false;

    // Main Game Loop
    // WindowShouldClose() returns true if pressing escape or close buton
    while (!WindowShouldClose()) { 
        float dt = GetFrameTime();

        if (!isGameOver){
            // Apply boost speed if active
            bg.scrollSpeed = itemSpawner.IsBoostActive() ? BOOST_BG_SPEED : NORMAL_BG_SPEED;
            bg.Update(dt);

            if (!kid.isDetached) {
                wau.Update(dt, screenWidth, screenHeight);
            } else {
                wau.pos.y += 400.0f * dt; 
            }

            kid.Update(dt, wau.pos);
            scoreManager.Update(dt, kid.isDetached);
            spawner.Update(dt, scoreManager.currentAltitude);
            itemSpawner.Update(dt);

            // Check item collisions only if the string is still attached
            if (!kid.isDetached) {
                Vector2 kidHitboxPos = { kid.pos.x - 15.0f, kid.pos.y };

                // Collect Bunga Raya
                int boost = itemSpawner.CheckBungaCollisions(wau.pos, wau.radius, kidHitboxPos, kid.radius);
                scoreManager.currentAltitude += boost; 
                
                // Collect Tali Tangsi
                int extraStrings = itemSpawner.CheckTangsiCollisions(wau.pos, wau.radius, kidHitboxPos, kid.radius);
                scoreManager.stringCharges += extraStrings;
            }

            // Check obstacle collisions only if the string is still attached
            if (!kid.isDetached && CollisionManager::CheckPlayerCollisions(wau, kid, spawner)) {
                kid.isDetached = true;
                
                // Give the kid a little visual bump when hitting an obstacle to emphasize the impact
                kid.velocity.y = -300.0f; 
                kid.velocity.x = -150.0f; 
            }

            // If the kid is falling, let the player click to create a new string and save him
            if (kid.isDetached && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && scoreManager.stringCharges > 0) {
                scoreManager.stringCharges--;
                kid.isDetached = false;
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
                itemSpawner.Reset();
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
                itemSpawner.Draw();
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
    itemSpawner.Unload();
    CloseWindow(); 

    return 0;
}

