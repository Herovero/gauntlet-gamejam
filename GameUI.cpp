#include "GameUI.hpp"
#include <cmath>

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

void GameUI::DrawItemBuffs(bool hasAisKacang, bool isSlowMoActive, bool isBoostActive, Texture2D texAisKacang, Texture2D texBungaRaya) {
    // Pulsing sine wave calculation (flashes smoothly between 30% and 100% opacity)
    float pulse = (std::sin((float)GetTime() * 10.0f) + 1.0f) / 2.0f;
    float alpha = 0.3f + (pulse * 0.7f);
    Color flashTint = Fade(WHITE, alpha);

    float startX = 280.0f; // Positioned right next to the HUD frame (width 260 + margin)
    float startY = 20.0f;
    float iconSize = 45.0f;

    // 1. Banked Ais Kacang Icon (Flashes when collected; disappears when slow motion is activated)
    if (hasAisKacang && !isSlowMoActive) {
        if (texAisKacang.id > 0) {
            Rectangle source = { 0.0f, 0.0f, (float)texAisKacang.width, (float)texAisKacang.height };
            Rectangle dest = { startX, startY, iconSize, iconSize };
            DrawTexturePro(texAisKacang, source, dest, { 0.0f, 0.0f }, 0.0f, flashTint);
            
            // Shift X rightwards so Bunga Raya can draw alongside if both are active
            startX += iconSize + 10.0f; 
        }
    }

    // 2. Active Bunga Raya Icon (Flashes while speed boost is active; disappears when boost expires)
    if (isBoostActive) {
        if (texBungaRaya.id > 0) {
            Rectangle source = { 0.0f, 0.0f, (float)texBungaRaya.width, (float)texBungaRaya.height };
            Rectangle dest = { startX, startY, iconSize, iconSize };
            DrawTexturePro(texBungaRaya, source, dest, { 0.0f, 0.0f }, 0.0f, flashTint);
        }
    }
}

bool GameUI::IsStartButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos) {
    Rectangle startBtn = { (float)screenWidth / 2.0f + 120.0f, (float)screenHeight / 2.0f, 260.0f, 50.0f };
    return CheckCollisionPointRec(virtualMousePos, startBtn);
}

bool GameUI::IsShopButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos) {
    Rectangle shopBtn = { (float)screenWidth / 2.0f + 120.0f, (float)screenHeight / 2.0f + 120.0f, 260.0f, 50.0f };
    return CheckCollisionPointRec(virtualMousePos, shopBtn);
}

bool GameUI::IsPauseButtonClicked(int screenWidth, Vector2 virtualMousePos) {
    Vector2 center = { (float)screenWidth - 60.0f, 60.0f };
    float radius = 32.0f;
    return CheckCollisionPointCircle(virtualMousePos, center, radius);
}

void GameUI::DrawPauseButton(int screenWidth, Vector2 virtualMousePos) {
    Vector2 center = { (float)screenWidth - 60.0f, 60.0f };
    float radius = 32.0f;
    
    bool isHovered = CheckCollisionPointCircle(virtualMousePos, center, radius);
    
    // Colors matching the reference image
    Color rimColor = GetColor(0xDF9C51FF);    // Light wood/orange rim
    Color innerColor = GetColor(0xD5661DFF);  // Vibrant orange center
    Color shadowColor = GetColor(0xA53A0BFF); // Deep inner shadow
    
    if (isHovered) {
        rimColor = GetColor(0xF0B062FF);
        innerColor = GetColor(0xE8792EFF);
    }

    // 1. Outer rim
    DrawCircleV(center, radius, rimColor);
    // 2. Inner shadow/depth offset slightly downwards
    DrawCircleV({center.x, center.y + 3.0f}, radius - 6.0f, shadowColor);
    // 3. Inner orange center
    DrawCircleV(center, radius - 6.0f, innerColor);

    // 4. Two white pause bars
    DrawRectangleRounded({ center.x - 10.0f, center.y - 12.0f, 7.0f, 24.0f }, 0.2f, 4, RAYWHITE);
    DrawRectangleRounded({ center.x + 3.0f, center.y - 12.0f, 7.0f, 24.0f }, 0.2f, 4, RAYWHITE);
}

bool GameUI::IsRestartButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos) {
    Rectangle btn = { (float)screenWidth / 2.0f - 100.0f, (float)screenHeight / 2.0f + 40.0f, 200.0f, 50.0f };
    return CheckCollisionPointRec(virtualMousePos, btn);
}

bool GameUI::IsMainMenuButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos) {
    Rectangle btn = { (float)screenWidth / 2.0f - 100.0f, (float)screenHeight / 2.0f + 105.0f, 200.0f, 50.0f };
    return CheckCollisionPointRec(virtualMousePos, btn);
}

bool GameUI::IsCreditsButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos) {
    Rectangle btn = { (float)screenWidth / 2.0f + 120.0f, (float)screenHeight / 2.0f + 180.0f, 260.0f, 50.0f };
    return CheckCollisionPointRec(virtualMousePos, btn);
}

bool GameUI::IsCreditsBackButtonClicked(int screenWidth, int screenHeight, Vector2 virtualMousePos) {
    Rectangle btn = { (float)screenWidth / 2.0f - 100.0f, (float)screenHeight / 2.0f + 220.0f, 200.0f, 46.0f };
    return CheckCollisionPointRec(virtualMousePos, btn);
}

void GameUI::DrawEndGameButtons(int screenWidth, int screenHeight, Vector2 virtualMousePos, bool isPaused) {
    // Restart / Resume Button (Styled with Wood & Gold)
    Rectangle restartBtn = { (float)screenWidth / 2.0f - 100.0f, (float)screenHeight / 2.0f + 40.0f, 200.0f, 46.0f };
    bool restartHover = IsRestartButtonClicked(screenWidth, screenHeight, virtualMousePos);
    Color restartBg = restartHover ? GetColor(0x8D6E63FF) : GetColor(0x4E342EFF);
    DrawRectangleRounded(restartBtn, 0.2f, 4, restartBg);
    DrawRectangleRoundedLines(restartBtn, 0.2f, 4, restartHover ? GOLD : GetColor(0xD4AF37FF)); 
    
    // Swap text based on the game state
    const char* topBtnText = isPaused ? "RESUME" : "RESTART";
    DrawText(topBtnText, screenWidth / 2 - MeasureText(topBtnText, 20) / 2, screenHeight / 2 + 53, 20, RAYWHITE);

    // Main Menu Button (Styled with Wood & Gold)
    Rectangle menuBtn = { (float)screenWidth / 2.0f - 100.0f, (float)screenHeight / 2.0f + 100.0f, 200.0f, 46.0f };
    bool menuHover = IsMainMenuButtonClicked(screenWidth, screenHeight, virtualMousePos);
    Color menuBg = menuHover ? GetColor(0x8D6E63FF) : GetColor(0x4E342EFF);
    DrawRectangleRounded(menuBtn, 0.2f, 4, menuBg);
    DrawRectangleRoundedLines(menuBtn, 0.2f, 4, menuHover ? GOLD : GetColor(0xD4AF37FF)); 
    DrawText("MAIN MENU", screenWidth / 2 - MeasureText("MAIN MENU", 20) / 2, screenHeight / 2 + 113, 20, RAYWHITE);
}

void GameUI::DrawMainMenu(int screenWidth, int screenHeight, Vector2 virtualMousePos, bool showUI, Texture2D titleTex) {
    if (!showUI) return; 

    int panelWidth = 620;
    int panelHeight = 400;
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
    Rectangle startBtn = { (float)screenWidth / 2.0f + 120.0f, (float)screenHeight / 2.0f, 260.0f, 46.0f };
    bool startHover = IsStartButtonClicked(screenWidth, screenHeight, virtualMousePos);
    Color startBg = startHover ? GetColor(0x8D6E63FF) : GetColor(0x5D4037FF);
    DrawRectangleRounded(startBtn, 0.2f, 4, startBg);
    DrawRectangleRoundedLines(startBtn, 0.2f, 4, startHover ? GOLD : GetColor(0xD4AF37FF)); // Removed thickness number!
    DrawText("START GAME", screenWidth / 2 + 250 - MeasureText("START GAME", 20) / 2, screenHeight / 2 + 13, 20, RAYWHITE);

    // Shop Button (Styled with Wood & Gold)
    Rectangle shopBtn = { (float)screenWidth / 2.0f + 120.0f, (float)screenHeight / 2.0f + 120.0f, 260.0f, 46.0f };
    bool shopHover = IsShopButtonClicked(screenWidth, screenHeight, virtualMousePos);
    Color shopBg = shopHover ? GetColor(0x8D6E63FF) : GetColor(0x5D4037FF);
    DrawRectangleRounded(shopBtn, 0.2f, 4, shopBg);
    DrawRectangleRoundedLines(shopBtn, 0.2f, 4, shopHover ? GOLD : GetColor(0xD4AF37FF));
    DrawText("OPEN SHOP", screenWidth / 2 + 250 - MeasureText("OPEN SHOP", 20) / 2, screenHeight / 2 + 133, 20, RAYWHITE);

    // Credits Button
    Rectangle creditsBtn = { (float)screenWidth / 2.0f + 120.0f, (float)screenHeight / 2.0f + 180.0f, 260.0f, 46.0f };
    bool creditsHover = IsCreditsButtonClicked(screenWidth, screenHeight, virtualMousePos);
    Color creditsBg = creditsHover ? GetColor(0x8D6E63FF) : GetColor(0x5D4037FF);
    DrawRectangleRounded(creditsBtn, 0.2f, 4, creditsBg);
    DrawRectangleRoundedLines(creditsBtn, 0.2f, 4, creditsHover ? GOLD : GetColor(0xD4AF37FF));
    DrawText("CREDITS", screenWidth / 2 + 250 - MeasureText("CREDITS", 20) / 2, screenHeight / 2 + 193, 20, RAYWHITE);
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

void GameUI::DrawCreditsScreen(int screenWidth, int screenHeight, Vector2 virtualMousePos, Texture2D githubLogo, Texture2D logo42KL, Texture2D logoLarian) {
    int panelWidth = 600;
    int panelHeight = 550;
    Rectangle panel = {
        (float)(screenWidth / 2 - panelWidth / 2),
        (float)(screenHeight / 2 - 250),
        (float)panelWidth,
        (float)panelHeight
    };

    DrawTraditionalFrame(panel);

    // Title
    DrawText("CREDITS", screenWidth / 2 - MeasureText("CREDITS", 40) / 2, panel.y + 30, 40, GOLD);

    // Developer Section
    DrawText("Developer:", screenWidth / 2 - MeasureText("Developer:", 24) / 2, panel.y + 90, 24, GetColor(0x3E2723FF));
    DrawText("Veron Siaw", screenWidth / 2 - MeasureText("Veron Siaw", 28) / 2, panel.y + 120, 28, DARKBLUE);
    
    // GitHub Logo and Handle (Fixed Placement)
    float ghSize = 40.0f;
    Rectangle ghSource = { 0.0f, 0.0f, (float)githubLogo.width, (float)githubLogo.height };
    Rectangle ghDest = { (float)screenWidth / 2 - 75, panel.y + 160, ghSize, ghSize }; 
    DrawTexturePro(githubLogo, ghSource, ghDest, { 0.0f, 0.0f }, 0.0f, WHITE);
    
    DrawText("Herovero", screenWidth / 2 - 25, panel.y + 168, 24, DARKBLUE);

    // Special Thanks Section
    DrawText("Special Thanks:", screenWidth / 2 - MeasureText("Special Thanks:", 24) / 2, panel.y + 240, 24, GetColor(0x3E2723FF));
    
    // 42KL Logo (Fixed Aspect Ratio and Size)
    float width42 = 130.0f;
    float height42 = width42 * ((float)logo42KL.height / (float)logo42KL.width);
    Rectangle source42 = { 0.0f, 0.0f, (float)logo42KL.width, (float)logo42KL.height };
    Rectangle dest42 = { (float)screenWidth / 2 - 170, panel.y + 290, width42, height42 };
    DrawTexturePro(logo42KL, source42, dest42, { 0.0f, 0.0f }, 0.0f, WHITE);

    // Larian Logo (Fixed Aspect Ratio and Size)
    float heightLarian = 160.0f;
    float widthLarian = heightLarian * ((float)logoLarian.width / (float)logoLarian.height);
    Rectangle sourceLarian = { 0.0f, 0.0f, (float)logoLarian.width, (float)logoLarian.height };
    Rectangle destLarian = { (float)screenWidth / 2 + 40, panel.y + 280, widthLarian, heightLarian };
    DrawTexturePro(logoLarian, sourceLarian, destLarian, { 0.0f, 0.0f }, 0.0f, WHITE);

    // Back Button
    Rectangle backBtn = { (float)screenWidth / 2.0f - 100.0f, panel.y + 470.0f, 200.0f, 46.0f };
    bool backHover = IsCreditsBackButtonClicked(screenWidth, screenHeight, virtualMousePos);
    Color backBg = backHover ? GetColor(0x8D6E63FF) : GetColor(0x5D4037FF);
    DrawRectangleRounded(backBtn, 0.2f, 4, backBg);
    DrawRectangleRoundedLines(backBtn, 0.2f, 4, backHover ? GOLD : GetColor(0xD4AF37FF));
    DrawText("BACK", screenWidth / 2 - MeasureText("BACK", 20) / 2, backBtn.y + 13, 20, RAYWHITE);
}