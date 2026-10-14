#include "VirtualJoystick.hpp"
#include <cmath>

VirtualJoystick::VirtualJoystick(float radius) {
    this->radius = radius;
    active = false;
    origin = { 0.0f, 0.0f };
    current = { 0.0f, 0.0f };
    input = { 0.0f, 0.0f };
}

void VirtualJoystick::Update() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        active = true;
        origin = GetMousePosition();
        current = origin;
        input = { 0.0f, 0.0f };
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && active) {
        current = GetMousePosition();
        
        float dx = current.x - origin.x;
        float dy = current.y - origin.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        
        // Clamp the knob visually to the max radius
        if (distance > radius) {
            current.x = origin.x + (dx / distance) * radius;
            current.y = origin.y + (dy / distance) * radius;
            distance = radius;
        }

        // Normalize input from -1.0 to 1.0
        if (distance > 0.0f) {
            input.x = (current.x - origin.x) / radius;
            input.y = (current.y - origin.y) / radius;
        } else {
            input = { 0.0f, 0.0f };
        }
    }
    
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        active = false;
        input = { 0.0f, 0.0f };
    }
}

void VirtualJoystick::Draw() {
    if (active) {
        DrawCircleV(origin, radius, Fade(WHITE, 0.3f));
        DrawCircleV(current, 35.0f, Fade(WHITE, 0.7f));
    }
}

bool VirtualJoystick::IsActive() const {
    return active;
}

Vector2 VirtualJoystick::GetInput() const {
    return input;
}

void VirtualJoystick::Reset() {
    active = false;
    input = { 0.0f, 0.0f };
}