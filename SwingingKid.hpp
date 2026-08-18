#pragma once
#include "raylib.h"

class SwingingKid {
    public:
        Vector2 pos;
        Vector2 velocity;
        float radius;
        float stringLength;
        float gravity;
        Texture2D texture;

        SwingingKid(Vector2 anchorPos, const char* imagePath);
        void Update(float dt, Vector2 anchorPos);
        void Draw(Vector2 anchorPos);
        void Unload();
};