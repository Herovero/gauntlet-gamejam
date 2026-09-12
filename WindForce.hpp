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
};