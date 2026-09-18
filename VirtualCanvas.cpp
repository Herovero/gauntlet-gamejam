#include "VirtualCanvas.hpp"

VirtualCanvas::VirtualCanvas(int width, int height) {
    screenWidth = width;
    screenHeight = height;
    target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
}

void VirtualCanvas::UpdateScaling() {
    scale = std::min((float)GetScreenWidth() / screenWidth, (float)GetScreenHeight() / screenHeight);
    offsetX = (GetScreenWidth() - ((float)screenWidth * scale)) * 0.5f;
    offsetY = (GetScreenHeight() - ((float)screenHeight * scale)) * 0.5f;
}

Vector2 VirtualCanvas::GetVirtualMousePosition() const {
    Vector2 rawMousePos = GetMousePosition();
    return { (rawMousePos.x - offsetX) / scale, (rawMousePos.y - offsetY) / scale };
}

void VirtualCanvas::BeginMode() {
    BeginTextureMode(target);
}

void VirtualCanvas::EndModeAndDraw() {
    EndTextureMode();
    
    BeginDrawing();
    ClearBackground(BLACK); // Black letterbox bars
    
    // Draw the scaled virtual canvas to the physical window
    DrawTexturePro(target.texture, 
        { 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
        { offsetX, offsetY, (float)screenWidth * scale, (float)screenHeight * scale }, 
        { 0, 0 }, 0.0f, WHITE);
    
    EndDrawing();
}

void VirtualCanvas::Unload() {
    UnloadRenderTexture(target);
}