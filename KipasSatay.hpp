#pragma once
#include "raylib.h"
#include "AudioManager.hpp"
#include <vector>

class KipasSatay {
private:
    std::vector<Texture2D> frames;
    int currentFrame;
    float frameTimer;
    int animDirection;
    float currentX;
    float targetX;
    bool lastWindFromLeft;
    bool lastActive;

public:
    KipasSatay();
    void Update(float dt, int screenWidth, bool isActive, bool isWindFromLeft, AudioManager& audio);
    void Draw(int screenHeight, bool isActive, bool isWindFromLeft);
    void Reset();
    void Unload();
};