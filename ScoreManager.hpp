#pragma once
#include "raylib.h"

class ScoreManager {
public:
    float currentAltitude;
    float highestAltitude;
    float baseClimbRate;
    float boostClimbRate;
    int stringCharges;

    Texture2D texTangsi;

    ScoreManager();
    void Update(float dt, bool isDetached, bool isBoosted);
    void Draw();
    void DrawGameOver(int screenWidth, int screenHeight);
    void Reset();
    void Unload();
};