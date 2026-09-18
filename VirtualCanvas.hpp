#pragma once
#include "raylib.h"
#include <algorithm>

class VirtualCanvas {
private:
    int screenWidth;
    int screenHeight;
    float scale;
    float offsetX;
    float offsetY;
    RenderTexture2D target;

public:
    VirtualCanvas(int width, int height);
    
    // Recalculates scale (call this every frame in case the window resizes)
    void UpdateScaling();
    
    // Returns the perfectly scaled mouse position
    Vector2 GetVirtualMousePosition() const;
    
    void BeginMode();
    void EndModeAndDraw();
    void Unload();
};