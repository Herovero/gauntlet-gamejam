#include "Background.hpp"

Background::Background(const char* imagePath, int screenWidth, int screenHeight, float scrollSpeed) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->scrollSpeed = scrollSpeed;
    this->texture = LoadTexture(imagePath);

    // Maintain the aspect ratio
    float scale = (float)screenWidth / (float)texture.width;
    drawWidth = (float)texture.width * scale;
    drawHeight = (float)texture.height * scale;
    
    // Center horizontally
    drawX = ((float)screenWidth - drawWidth) / 2.0f;

    // Start at the bottom
    scrollY = (float)screenHeight - drawHeight;
}

void Background::Update(float dt) {
    scrollY += scrollSpeed * dt;
}

void Background::Draw() {
    Rectangle src = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };

    // Draws the image on screen without stretching or distorting it
    Rectangle dest = { drawX, scrollY, drawWidth, drawHeight };
    DrawTexturePro(texture, src, dest, { 0.0f, 0.0f }, 0.0f, WHITE);

    // Loop a copy above
    if (scrollY > (float)screenHeight - drawHeight) {
        Rectangle destLoop = { drawX, scrollY - drawHeight, drawWidth, drawHeight };
        DrawTexturePro(texture, src, destLoop, { 0.0f, 0.0f }, 0.0f, WHITE);
    }
}

void Background::Unload() {
    UnloadTexture(texture);
}