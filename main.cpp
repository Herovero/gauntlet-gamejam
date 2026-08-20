#include "raylib.h"
#include "WauBulan.hpp"
#include "SwingingKid.hpp"
#include "Obstacle.hpp"
#include "Background.hpp"
#include "ObstacleSpawner.hpp"
#include "CollisionManager.hpp"
#include "ScoreManager.hpp"
#include "ItemSpawner.hpp"

enum GameState {
    MENU,
    PLAYING,
    GAMEOVER
};

int main() {
    // Screen Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;

    // Run the window and show title
    InitWindow(screenWidth, screenHeight, "Wau Bulan Rising");
    
    // Initialize audio to load mp3
    InitAudioDevice();
    Music bgm = LoadMusicStream("assets/bgm.mp3");
    SetMusicVolume(bgm, 1.0f);

    Sound sfxHit = LoadSound("assets/hit.wav");
    SetSoundVolume(sfxHit, 0.8f);

    Sound sfxItem = LoadSound("assets/pickup.wav");
    SetSoundVolume(sfxItem, 0.9f);

    // Framerate per second
    SetTargetFPS(60); 

    Background bg("assets/background.png", screenWidth, screenHeight, 30.0f);

    WauBulan wau(screenWidth / 2.0f, screenHeight - 600.0f, "assets/waubulan.png");
    SwingingKid kid(wau.pos, "assets/kid_swinging.png", "assets/kid_falling.png", "assets/kid_standing.png");
    ObstacleSpawner spawner(screenWidth, screenHeight);
    ItemSpawner itemSpawner(screenWidth, screenHeight);
    ScoreManager scoreManager;

    const float NORMAL_BG_SPEED = 30.0f;
    const float BOOST_BG_SPEED = 150.0f;

    // Initialize State Machine
    GameState gameState = MENU;

    // Set up the menu positions
    kid.isOnGround = true;
    kid.pos = { (float)screenWidth / 2.0f, (float)screenHeight - 50.0f }; // Rest on ground
    wau.pos = { (float)screenWidth / 2.0f, (float)screenHeight - 600.0f }; // Hover in sky

    // Main Game Loop
    // WindowShouldClose() returns true if pressing escape or close buton
    while (!WindowShouldClose()) { 
        float dt = GetFrameTime();

        UpdateMusicStream(bgm);

        if (gameState == MENU) {
            // In the menu wait for input, Background stays still
            if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                gameState = PLAYING;
                kid.isOnGround = false; // Turn on gravity and string physics

                PlayMusicStream(bgm);
            }
        }
        else if (gameState == PLAYING) {
            // Apply boost speed if active
            bg.scrollSpeed = itemSpawner.IsBoostActive() ? BOOST_BG_SPEED : NORMAL_BG_SPEED;
            bg.Update(dt);

            if (!kid.isDetached) wau.Update(dt, screenWidth, screenHeight);
            else wau.pos.y += 400.0f * dt;

            kid.Update(dt, wau.pos);
            scoreManager.Update(dt, kid.isDetached, itemSpawner.IsBoostActive());
            spawner.Update(dt, scoreManager.currentAltitude);
            itemSpawner.Update(dt);

            // Check item collisions only if the string is still attached
            if (!kid.isDetached) {
                Vector2 kidHitboxPos = { kid.pos.x - 15.0f, kid.pos.y };

                // Collect Bunga Raya
                int boost = itemSpawner.CheckBungaCollisions(wau.pos, wau.radius, kidHitboxPos, kid.radius);
                if (boost > 0) {
                    scoreManager.currentAltitude += boost;
                    PlaySound(sfxItem);
                }
                
                // Collect Tali Tangsi
                int extraStrings = itemSpawner.CheckTangsiCollisions(wau.pos, wau.radius, kidHitboxPos, kid.radius);
                if (extraStrings > 0) {
                    scoreManager.stringCharges += extraStrings;
                    PlaySound(sfxItem);
                }
            }

            // Check obstacle collisions only if the string is still attached
            if (!kid.isDetached && wau.invincibleTimer <= 0.0f && CollisionManager::CheckPlayerCollisions(wau, kid, spawner)) {
                PlaySound(sfxHit);
                kid.isDetached = true;
                
                // Give the kid a little visual bump when hitting an obstacle to emphasize the impact
                kid.velocity.y = -300.0f; 
                kid.velocity.x = -150.0f;
            }

            // If the kid is falling, let the player click to create a new string and save him
            if (kid.isDetached && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && scoreManager.stringCharges > 0) {
                // Get mouse position
                Vector2 mousePos = GetMousePosition();

                // Check if the click happened inside the Wau Bulan's hitbox
                if (CheckCollisionPointCircle(mousePos, wau.pos, wau.radius * 3.0f)) {
                    scoreManager.stringCharges--;
                    kid.isDetached = false;

                    wau.invincibleTimer = 2.0f;
                }
            }

            // Trigger the game over screen when the kid drops out of view
            if (kid.isDetached && (kid.pos.y - kid.radius) > (float)screenHeight) {
                gameState = GAMEOVER;

                StopMusicStream(bgm);
            }
        }
        else if (gameState == GAMEOVER) {
            if (IsKeyPressed(KEY_SPACE)) {
                gameState = MENU;
                bg.Reset();
                wau.pos = { (float)screenWidth / 2.0f, (float)screenHeight - 600.0f };

                kid.pos = { wau.pos.x, wau.pos.y + 120.0f };
                kid.velocity = { 0.0f, 0.0f };
                kid.isDetached = false;

                spawner.Reset();
                itemSpawner.Reset();
                scoreManager.Reset();

                // Reset to main menu positions
                kid.isDetached = false;
                kid.isOnGround = true;
                kid.velocity = { 0.0f, 0.0f };
                kid.pos = { (float)screenWidth / 2.0f, (float)screenHeight - 50.0f };
                wau.pos = { (float)screenWidth / 2.0f, (float)screenHeight - 600.0f };
            }
        }

        // Drawing logic
        BeginDrawing();
            // Switch background color
            ClearBackground(SKYBLUE);
            bg.Draw();

            if (gameState == MENU) {
                wau.Draw();
                kid.Draw(wau.pos);

                const char* title = "WAU BULAN RISING";
                const char* start = "Press SPACE to Take Off!";
                DrawText(title, screenWidth / 2 - MeasureText(title, 60) / 2, screenHeight / 2 - 100, 60, DARKBLUE);
                DrawText(start, screenWidth / 2 - MeasureText(start, 30) / 2, screenHeight / 2, 30, YELLOW);
            }
            else if (gameState == PLAYING) {
                wau.Draw();
                kid.Draw(wau.pos);
                spawner.Draw();
                itemSpawner.Draw();
                scoreManager.Draw();
            } 
            else if (gameState == GAMEOVER) {
                scoreManager.DrawGameOver(screenWidth, screenHeight);
            }

        EndDrawing();
    }

    wau.Unload();
    kid.Unload();
    bg.Unload();
    spawner.Unload();
    itemSpawner.Unload();

    UnloadSound(sfxHit);
    UnloadSound(sfxItem);
    UnloadMusicStream(bgm);
    CloseAudioDevice();
    CloseWindow(); 

    return 0;
}

