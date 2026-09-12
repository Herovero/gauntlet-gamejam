#pragma once
#include "raylib.h"
#include <vector>

class KipasSatay {
private:
    std::vector<Texture2D> frames;
    int currentFrame;
    float frameTimer;
    int animDirection;
    float currentX;
    float targetX;

public:
    KipasSatay();
    void Update(float dt, int screenWidth, bool isActive, bool isWindFromLeft);
    void Draw(int screenHeight, bool isActive, bool isWindFromLeft);
    void Reset();
    void Unload();
};