#include "GameUI.hpp"

void GameUI::DrawTraditionalFrame(Rectangle bounds) {
    // Colors
    Color paperBg    = Fade(GetColor(0xFDF8EEFF), 0.88f); // Warm translucent rice paper
    Color goldBorder = GetColor(0xD4AF37FF);              // Songket gold
    Color innerTrim  = Fade(GetColor(0x8D6E63FF), 0.45f); // Carved bamboo brown
    Color woodShadow = Fade(GetColor(0x2C1D11FF), 0.35f); // Soft outer drop shadow

    // 1. Soft Outer Shadow
    Rectangle shadowRec = { bounds.x + 4.0f, bounds.y + 4.0f, bounds.width, bounds.height };
    DrawRectangleRounded(shadowRec, 0.04f, 4, woodShadow);

    // 2. Base Rice Paper Panel
    DrawRectangleRounded(bounds, 0.04f, 4, paperBg);

    // 3. Primary Gold Outer Border
    DrawRectangleRoundedLines(bounds, 0.04f, 4, goldBorder);

    // 4. Subtle Inner Bamboo Filigree Line
    Rectangle innerRec = { bounds.x + 6.0f, bounds.y + 6.0f, bounds.width - 12.0f, bounds.height - 12.0f };
    DrawRectangleRoundedLines(innerRec, 0.03f, 4, innerTrim);

    // 5. Ornate Diamond Corner Studs (Bucu Intan)
    Vector2 corners[4] = {
        { bounds.x + 8.0f, bounds.y + 8.0f },                                    // Top-Left
        { bounds.x + bounds.width - 8.0f, bounds.y + 8.0f },                     // Top-Right
        { bounds.x + bounds.width - 8.0f, bounds.y + bounds.height - 8.0f },     // Bottom-Right
        { bounds.x + 8.0f, bounds.y + bounds.height - 8.0f }                     // Bottom-Left
    };

    for (int i = 0; i < 4; i++) {
        DrawPoly(corners[i], 4, 5.0f, 45.0f, goldBorder);
    }
}

bool GameUI::IsStartButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos) {
    Rectangle startBtn = { (float)screenWidth / 2.0f + 150.0f, (float)screenHeight / 2.0f, 200.0f, 50.0f };
    return CheckCollisionPointRec(virtualMousePos, startBtn);
}

bool GameUI::IsShopButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos) {
    Rectangle shopBtn = { (float)screenWidth / 2.0f + 150.0f, (float)screenHeight / 2.0f + 70.0f, 200.0f, 50.0f };
    return CheckCollisionPointRec(virtualMousePos, shopBtn);
}

bool GameUI::IsRestartButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos) {
    Rectangle btn = { (float)screenWidth / 2.0f - 100.0f, (float)screenHeight / 2.0f + 40.0f, 200.0f, 50.0f };
    return CheckCollisionPointRec(virtualMousePos, btn);
}

bool GameUI::IsMainMenuButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos) {
    Rectangle btn = { (float)screenWidth / 2.0f - 100.0f, (float)screenHeight / 2.0f + 105.0f, 200.0f, 50.0f };
    return CheckCollisionPointRec(virtualMousePos, btn);
}

void GameUI::DrawEndGameButtons(int screenWidth, int screenHeight, Vector2 virtualMousePos) {
    // Restart Button (Styled with Wood & Gold)
    Rectangle restartBtn = { (float)screenWidth / 2.0f - 100.0f, (float)screenHeight / 2.0f + 40.0f, 200.0f, 46.0f };
    bool restartHover = IsRestartButtonClicked(screenWidth, screenHeight, virtualMousePos);
    Color restartBg = restartHover ? GetColor(0x8D6E63FF) : GetColor(0x4E342EFF);
    DrawRectangleRounded(restartBtn, 0.2f, 4, restartBg);
    DrawRectangleRoundedLines(restartBtn, 0.2f, 4, restartHover ? GOLD : GetColor(0xD4AF37FF)); // Removed thickness number!
    DrawText("RESTART", screenWidth / 2 - MeasureText("RESTART", 20) / 2, screenHeight / 2 + 53, 20, RAYWHITE);

    // Main Menu Button (Styled with Wood & Gold)
    Rectangle menuBtn = { (float)screenWidth / 2.0f - 100.0f, (float)screenHeight / 2.0f + 100.0f, 200.0f, 46.0f };
    bool menuHover = IsMainMenuButtonClicked(screenWidth, screenHeight, virtualMousePos);
    Color menuBg = menuHover ? GetColor(0x8D6E63FF) : GetColor(0x4E342EFF);
    DrawRectangleRounded(menuBtn, 0.2f, 4, menuBg);
    DrawRectangleRoundedLines(menuBtn, 0.2f, 4, menuHover ? GOLD : GetColor(0xD4AF37FF)); // Removed thickness number!
    DrawText("MAIN MENU", screenWidth / 2 - MeasureText("MAIN MENU", 20) / 2, screenHeight / 2 + 113, 20, RAYWHITE);
}

void GameUI::DrawMainMenu(int screenWidth, int screenHeight, Vector2 virtualMousePos, bool showUI, Texture2D titleTex) {
    if (!showUI) return; 

    int panelWidth = 620;
    int panelHeight = 240;
    Rectangle menuPanel = {
        (float)(screenWidth / 2 - panelWidth / 2 + 250),
        (float)(screenHeight / 2 - 130),
        (float)panelWidth,
        (float)panelHeight
    };

    // Replace the black box with the new custom frame!
    DrawTraditionalFrame(menuPanel);

    // Scale down the title image by 45% so it fits beautifully
    float titleScale = 0.45f; 
    float scaledWidth = titleTex.width * titleScale;
    
    // Center the scaled image relative to the shifted panel
    int titleX = (screenWidth / 2 + 250) - (int)(scaledWidth / 2.0f);
    int titleY = screenHeight / 2 - 100; 
    
    // Draw using DrawTextureEx to apply the scale
    DrawTextureEx(titleTex, { (float)titleX, (float)titleY }, 0.0f, titleScale, WHITE);

    // Start Button (Styled with Wood & Gold)
    Rectangle startBtn = { (float)screenWidth / 2.0f + 150.0f, (float)screenHeight / 2.0f, 200.0f, 46.0f };
    bool startHover = IsStartButtonClicked(screenWidth, screenHeight, virtualMousePos);
    Color startBg = startHover ? GetColor(0x8D6E63FF) : GetColor(0x5D4037FF);
    DrawRectangleRounded(startBtn, 0.2f, 4, startBg);
    DrawRectangleRoundedLines(startBtn, 0.2f, 4, startHover ? GOLD : GetColor(0xD4AF37FF)); // Removed thickness number!
    DrawText("START GAME", screenWidth / 2 + 250 - MeasureText("START GAME", 20) / 2, screenHeight / 2 + 13, 20, RAYWHITE);

    // Shop Button (Styled with Wood & Gold)
    Rectangle shopBtn = { (float)screenWidth / 2.0f + 150.0f, (float)screenHeight / 2.0f + 64.0f, 200.0f, 46.0f };
    bool shopHover = IsShopButtonClicked(screenWidth, screenHeight, virtualMousePos);
    Color shopBg = shopHover ? GetColor(0x8D6E63FF) : GetColor(0x5D4037FF);
    DrawRectangleRounded(shopBtn, 0.2f, 4, shopBg);
    DrawRectangleRoundedLines(shopBtn, 0.2f, 4, shopHover ? GOLD : GetColor(0xD4AF37FF)); // Removed thickness number!
    DrawText("OPEN SHOP", screenWidth / 2 + 250 - MeasureText("OPEN SHOP", 20) / 2, screenHeight / 2 + 77, 20, RAYWHITE);
}

void GameUI::DrawVictoryScreen(int screenWidth, int screenHeight) {
    int panelWidth = 540;
    int panelHeight = 280;
    Rectangle victPanel = {
        (float)(screenWidth / 2 - panelWidth / 2),
        (float)(screenHeight / 2 - 120),
        (float)panelWidth,
        (float)panelHeight
    };

    // Replace the black box with the new custom frame!
    DrawTraditionalFrame(victPanel);

    // Changed colors to match the warmer palette
    DrawText("MERDEKA!", screenWidth / 2 - MeasureText("MERDEKA!", 56) / 2, screenHeight / 2 - 90, 56, GetColor(0xB8860BFF));
    DrawText("You Reached the Top!", screenWidth / 2 - MeasureText("You Reached the Top!", 26) / 2, screenHeight / 2 - 18, 26, GetColor(0x3E2723FF));
}