#pragma once
#include "raylib.h"

class ScoreManager {
public:
    float currentAltitude;
    float highestAltitude;
    float climbRate;

    ScoreManager();
    void Update(float dt, bool isDetached);
    void Draw();
    void DrawGameOver(int screenWidth, int screenHeight);
    void Reset();
};