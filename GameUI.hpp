#pragma once
#include "raylib.h"

class GameUI {
public:
    static bool IsStartButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos);
    static bool IsShopButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos);

    static bool IsRestartButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos);
    static bool IsMainMenuButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos);
    static void DrawEndGameButtons(int screenWidth, int screenHeight, Vector2 virtualMousePos);
    
    static void DrawMainMenu(int screenWidth, int screenHeight, Vector2 virtualMousePos, bool showUI, Texture2D titleTex);
    static void DrawVictoryScreen(int screenWidth, int screenHeight);
};