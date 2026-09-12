#pragma once
#include "raylib.h"

class WindForce {
public:
    float currentWindForce;
    float targetWindForce;
    float activeWindTimer;
    float nextWindTimer;

    WindForce();
    void Update(float dt);
    void Draw(int screenWidth);
    float GetForce() const;
    void Reset();

    bool IsActive() const { return activeWindTimer > 0.0f; }
    bool IsWindFromLeft() const { return targetWindForce > 0.0f; }
};