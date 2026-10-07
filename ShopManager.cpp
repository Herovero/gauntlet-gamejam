#include "ShopManager.hpp"
#include <cmath>

ShopManager::ShopManager() {
    currentIndex = 0;
    equippedIndex = 0;
    currentX = 1280.0f / 2.0f;
    targetX = currentX;

    bgTexture = LoadTexture("assets/shop_bg.png");

    uiCoinIcon = LoadTexture("assets/coin.png");
    GenTextureMipmaps(&uiCoinIcon);
    SetTextureFilter(uiCoinIcon, TEXTURE_FILTER_TRILINEAR);

    // Load textures and apply filters
    Texture2D t1 = LoadTexture("assets/waubulan.png");
    GenTextureMipmaps(&t1); SetTextureFilter(t1, TEXTURE_FILTER_TRILINEAR);
    
    Texture2D t2 = LoadTexture("assets/waubulan2.png");
    GenTextureMipmaps(&t2); SetTextureFilter(t2, TEXTURE_FILTER_TRILINEAR);
    
    Texture2D t3 = LoadTexture("assets/waubulan3.png");
    GenTextureMipmaps(&t3); SetTextureFilter(t3, TEXTURE_FILTER_TRILINEAR);

    Texture2D t4 = LoadTexture("assets/waubulan4.png");
    GenTextureMipmaps(&t4); SetTextureFilter(t4, TEXTURE_FILTER_TRILINEAR);

    Texture2D t5 = LoadTexture("assets/waubulan5.png");
    GenTextureMipmaps(&t5); SetTextureFilter(t5, TEXTURE_FILTER_TRILINEAR);

    kites.push_back({"Wau Klasik", "assets/waubulan.png", t1, 250.0f, 1.0f, 0, true});
    kites.push_back({"Wau Perintis", "assets/waubulan2.png", t2, 320.0f, 1.2f, 3, false});
    kites.push_back({"Wau Samudera", "assets/waubulan3.png", t3, 370.0f, 1.1f, 5, false});
    kites.push_back({"Wau Zamrud", "assets/waubulan4.png", t4, 400.0f, 0.8f, 10, false});
    kites.push_back({"Wau Purnama", "assets/waubulan5.png", t5, 500.0f, 1.5f, 20, false});
}

void ShopManager::Update(int& playerCoins, bool& returnToMenu, float dt, int screenWidth, Vector2 virtualMousePos) {
    targetX = (float)screenWidth / 2.0f;
    
    // Define mobile-friendly button hitboxes
    Rectangle leftBtn   = { (float)screenWidth / 2.0f - 280.0f, 400.0f, 60.0f, 60.0f };
    Rectangle rightBtn  = { (float)screenWidth / 2.0f + 220.0f, 400.0f, 60.0f, 60.0f };
    Rectangle actionBtn = { (float)screenWidth / 2.0f - 125.0f, (float)screenWidth * 0.45f, 250.0f, 50.0f };
    Rectangle backBtn   = { 30.0f, 30.0f, 120.0f, 45.0f }; 

    bool clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    // Navigation (Keyboard OR Touch/Click)
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D) || (clicked && CheckCollisionPointRec(virtualMousePos, rightBtn))) {
        currentIndex++;
        if (currentIndex >= (int)kites.size()) currentIndex = 0;
        currentX = (float)screenWidth + 300.0f; 
    }
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A) || (clicked && CheckCollisionPointRec(virtualMousePos, leftBtn))) {
        currentIndex--;
        if (currentIndex < 0) currentIndex = (int)kites.size() - 1;
        currentX = -300.0f; 
    }

    currentX += (targetX - currentX) * 12.0f * dt;

    // Equip / Buy Logic
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || (clicked && CheckCollisionPointRec(virtualMousePos, actionBtn))) {
        // Only trigger if it's NOT already equipped
        if (currentIndex != equippedIndex) {
            if (kites[currentIndex].isUnlocked) {
                equippedIndex = currentIndex;
            } else if (playerCoins >= kites[currentIndex].price) {
                playerCoins -= kites[currentIndex].price;
                kites[currentIndex].isUnlocked = true;
                equippedIndex = currentIndex;
            }
        }
    }

    // Exit Shop
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE) || (clicked && CheckCollisionPointRec(virtualMousePos, backBtn))) {
        returnToMenu = true;
    }
}

void ShopManager::Draw(int screenWidth, int screenHeight, int playerCoins, Vector2 virtualMousePos) {
    if (bgTexture.id > 0) DrawTexture(bgTexture, 0, 0, WHITE);
    else ClearBackground(DARKBLUE); 

    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.5f));

    KiteProfile current = kites[currentIndex];

    // --- 1. Top HUD (Back Button & Coin Texture) ---
    Rectangle backBtn = { 30.0f, 30.0f, 120.0f, 45.0f };
    bool backHover = CheckCollisionPointRec(virtualMousePos, backBtn);
    DrawRectangleRounded(backBtn, 0.2f, 4, backHover ? GetColor(0x8D6E63FF) : GetColor(0x5D4037FF));
    DrawRectangleRoundedLines(backBtn, 0.2f, 4, GetColor(0xD4AF37FF));
    DrawText("< BACK", 45, 42, 20, RAYWHITE);

    // Draw Coin Texture and count
    DrawText("SHOP", screenWidth / 2 - MeasureText("SHOP", 40) / 2, 40, 40, GOLD);
    
    // Draw Coin Texture and count (Matching CoinManager exactly)
    float xPos = (float)screenWidth - 150.0f;
    float yPos = 30.0f;

    if (uiCoinIcon.id > 0) {
        float renderSize = 70.0f;
        Rectangle source = { 0.0f, 0.0f, (float)uiCoinIcon.width, (float)uiCoinIcon.height };
        Rectangle dest = { xPos, yPos, renderSize, renderSize };
        DrawTexturePro(uiCoinIcon, source, dest, { 0.0f, 0.0f }, 0.0f, WHITE);
    } else {
        DrawCircleV({ xPos + 15.0f, yPos + 15.0f }, 15.0f, MAGENTA);
    }

    std::string coinText = "x " + std::to_string(playerCoins);
    DrawText(coinText.c_str(), (int)xPos + 90, (int)yPos + 20, 24, GOLD);

    // --- 2. Kite Display & Stats ---
    DrawText(current.name.c_str(), screenWidth / 2 - MeasureText(current.name.c_str(), 50) / 2, 130, 50, WHITE);
    DrawText(TextFormat("Speed: %.0f", current.speed), screenWidth / 2 - 100, 220, 25, RAYWHITE);
    DrawText(TextFormat("Size: %.1fx", current.sizeMultiplier), screenWidth / 2 - 100, 260, 25, RAYWHITE);

    if (current.texture.id > 0) {
        float renderWidth = 200.0f * current.sizeMultiplier; 
        float aspectRatio = (float)current.texture.height / (float)current.texture.width;
        float renderHeight = renderWidth * aspectRatio;

        float hoverY = std::sin((float)GetTime() * 4.0f) * 15.0f;
        float drawY = (screenHeight / 2.0f) + 50.0f + hoverY;

        Rectangle source = { 0.0f, 0.0f, (float)current.texture.width, (float)current.texture.height };
        Rectangle dest = { currentX, drawY, renderWidth, renderHeight };
        Vector2 origin = { renderWidth / 2.0f, renderHeight / 2.0f };

        DrawTexturePro(current.texture, source, dest, origin, 0.0f, WHITE);
    }

    // --- 3. Arrow Buttons ---
    Rectangle leftBtn  = { (float)screenWidth / 2.0f - 280.0f, 400.0f, 60.0f, 60.0f };
    Rectangle rightBtn = { (float)screenWidth / 2.0f + 220.0f, 400.0f, 60.0f, 60.0f };
    
    DrawRectangleRounded(leftBtn, 0.2f, 4, CheckCollisionPointRec(virtualMousePos, leftBtn) ? GetColor(0x8D6E63FF) : GetColor(0x5D4037FF));
    DrawRectangleRoundedLines(leftBtn, 0.2f, 4, GetColor(0xD4AF37FF));
    DrawText("<", leftBtn.x + 20, leftBtn.y + 15, 30, RAYWHITE);

    DrawRectangleRounded(rightBtn, 0.2f, 4, CheckCollisionPointRec(virtualMousePos, rightBtn) ? GetColor(0x8D6E63FF) : GetColor(0x5D4037FF));
    DrawRectangleRoundedLines(rightBtn, 0.2f, 4, GetColor(0xD4AF37FF));
    DrawText(">", rightBtn.x + 20, rightBtn.y + 15, 30, RAYWHITE);

    // --- 4. Main Action Button (Equip / Buy) ---
    Rectangle actionBtn = { (float)screenWidth / 2.0f - 125.0f, (float)screenHeight - 120.0f, 250.0f, 50.0f };
    bool actionHover = CheckCollisionPointRec(virtualMousePos, actionBtn);
    
    Color btnColor;
    const char* btnText;
    Color textColor = RAYWHITE;

    if (currentIndex == equippedIndex) {
        btnColor = GetColor(0x2C2C2CFF); // Dark Gray (Disabled)
        btnText = "EQUIPPED";
        textColor = GRAY;
        actionHover = false; // Disable hover effect
    } else if (current.isUnlocked) {
        btnColor = actionHover ? GetColor(0x2E7D32FF) : GetColor(0x1B5E20FF); // Green
        btnText = "EQUIP";
    } else {
        btnColor = (playerCoins >= current.price) ? (actionHover ? GetColor(0x1565C0FF) : GetColor(0x0D47A1FF)) : GetColor(0x8B0000FF);
        btnText = TextFormat("BUY: %d COINS", current.price);
    }

    DrawRectangleRounded(actionBtn, 0.2f, 4, btnColor);
    DrawRectangleRoundedLines(actionBtn, 0.2f, 4, actionHover ? GOLD : GetColor(0xD4AF37FF));
    DrawText(btnText, screenWidth / 2 - MeasureText(btnText, 22) / 2, actionBtn.y + 14, 22, textColor);
}

KiteProfile ShopManager::GetEquippedKite() {
    return kites[equippedIndex];
}

void ShopManager::Unload() {
    if (bgTexture.id > 0) UnloadTexture(bgTexture);
    if (uiCoinIcon.id > 0) UnloadTexture(uiCoinIcon); // Add this line
    
    for (auto& kite : kites) {
        if (kite.texture.id > 0) UnloadTexture(kite.texture);
    }
}