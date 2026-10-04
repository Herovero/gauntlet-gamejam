#pragma once
#include "raylib.h"

class GameUI {
public:
    static bool IsStartButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos);
    static bool IsShopButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos);
    static void DrawMainMenu(int screenWidth, int screenHeight, Vector2 virtualMousePos, bool showUI);
    static void DrawVictoryScreen(int screenWidth, int screenHeight);
};