#pragma once
#include "raylib.h"

class ScoreManager {
public:
    float currentAltitude;
    float highestAltitude;
    float baseClimbRate;
    float boostClimbRate;
    int stringCharges;

    ScoreManager();
    void Update(float dt, bool isDetached, bool isBoosted);
    void Draw();
    void DrawGameOver(int screenWidth, int screenHeight);
    void Reset();
};