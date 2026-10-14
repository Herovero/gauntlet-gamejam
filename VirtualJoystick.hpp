#ifndef VIRTUAL_JOYSTICK_HPP
#define VIRTUAL_JOYSTICK_HPP

#include "raylib.h"

class VirtualJoystick {
public:
    VirtualJoystick(float radius = 100.0f);

    void Update();
    void Draw();

    bool IsActive() const;
    Vector2 GetInput() const; // Returns a normalized vector (-1.0 to 1.0)

    void Reset();

private:
    bool active;
    Vector2 origin;
    Vector2 current;
    float radius;
    Vector2 input;
};

#endif