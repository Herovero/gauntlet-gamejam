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
        Texture2D texFalling;

        bool isDetached;

        SwingingKid(Vector2 anchorPos, const char* normalPath, const char* fallingPath, float initialStringLength);
        void Update(float dt, Vector2 anchorPos, float windForce);
        void Draw(Vector2 anchorPos, float invincibleTimer = 0.0f);
        void Detach(float wauPosX);
        void TryReattach(Vector2 virtualMousePos, float wauPosX, float wauPosY, float wauRadius, float& wauInvincibleTimer, int& stringCharges);
        void Unload();
};