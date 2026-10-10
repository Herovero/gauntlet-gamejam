#include "AisKacang.hpp"
#include <cmath>

AisKacang::AisKacang(int screenWidth, Texture2D tex) {
    texture = tex;
    radius = 20.0f;
    pos = { (float)GetRandomValue(100, screenWidth - 100), -50.0f };
    fallSpeed = 200.0f;
}

void AisKacang::Update(float dt) {
    pos.y += fallSpeed * dt;
}

void AisKacang::Draw() {
    Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    float renderSize = radius * 2.5f;
    Rectangle dest = { pos.x, pos.y, renderSize, renderSize };
    Vector2 origin = { renderSize / 2.0f, renderSize / 2.0f };
    DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
}

bool AisKacang::CheckCollision(Vector2 playerPos, float playerRadius) {
    float dx = pos.x - playerPos.x;
    float dy = pos.y - playerPos.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance <= (radius + playerRadius);
}