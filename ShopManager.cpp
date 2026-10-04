#include "ShopManager.hpp"
#include <cmath>

ShopManager::ShopManager() {
    currentIndex = 0;
    equippedIndex = 0;
    currentX = 1280.0f / 2.0f;
    targetX = currentX;

    bgTexture = LoadTexture("assets/shop_bg.png");

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

void ShopManager::Update(int& playerCoins, bool& returnToMenu, float dt, int screenWidth) {
    targetX = (float)screenWidth / 2.0f;
    
    // Navigation with instant snap for the tween
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        currentIndex++;
        if (currentIndex >= (int)kites.size()) currentIndex = 0;
        currentX = (float)screenWidth + 300.0f; // Snap off-screen right
    }
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        currentIndex--;
        if (currentIndex < 0) currentIndex = (int)kites.size() - 1;
        currentX = -300.0f; // Snap off-screen left
    }

    // Smoothly interpolate currentX to the center of the screen
    currentX += (targetX - currentX) * 12.0f * dt;

    // Buy or Equip (Space or Enter)
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        if (kites[currentIndex].isUnlocked) {
            equippedIndex = currentIndex;
        } else if (playerCoins >= kites[currentIndex].price) {
            playerCoins -= kites[currentIndex].price;
            kites[currentIndex].isUnlocked = true;
            equippedIndex = currentIndex;
        }
    }

    // Exit Shop
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) {
        returnToMenu = true;
    }
}

void ShopManager::Draw(int screenWidth, int screenHeight, int playerCoins) {
    // Draw the background image starting at top-left corner (0, 0)
    if (bgTexture.id > 0) {
        DrawTexture(bgTexture, 0, 0, WHITE);
    } else {
        ClearBackground(DARKBLUE); // Fallback just in case it fails to load
    }

    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.5f));

    KiteProfile current = kites[currentIndex];

    // Header
    DrawText("SHOP", screenWidth / 2 - MeasureText("SHOP", 40) / 2, 50, 40, GOLD);
    DrawText(TextFormat("Coins: %d", playerCoins), 50, 50, 30, YELLOW);

    // Kite Name and Stats
    DrawText(current.name.c_str(), screenWidth / 2 - MeasureText(current.name.c_str(), 50) / 2, 150, 50, WHITE);
    DrawText(TextFormat("Speed: %.0f", current.speed), screenWidth / 2 - 100, 250, 25, RAYWHITE);
    DrawText(TextFormat("Size: %.1fx", current.sizeMultiplier), screenWidth / 2 - 100, 290, 25, RAYWHITE);

    // Draw the hovering kite
    if (current.texture.id > 0) {
        float renderWidth = 200.0f * current.sizeMultiplier; 
        float aspectRatio = (float)current.texture.height / (float)current.texture.width;
        float renderHeight = renderWidth * aspectRatio;

        // Sine wave hover effect (moves up and down smoothly)
        float hoverY = std::sin((float)GetTime() * 4.0f) * 15.0f;
        
        // Position it right in the empty space between the stats and the purchase text
        float drawY = (screenHeight / 2.0f) + 80.0f + hoverY;

        Rectangle source = { 0.0f, 0.0f, (float)current.texture.width, (float)current.texture.height };
        Rectangle dest = { currentX, drawY, renderWidth, renderHeight };
        Vector2 origin = { renderWidth / 2.0f, renderHeight / 2.0f };

        DrawTexturePro(current.texture, source, dest, origin, 0.0f, WHITE);
    }

    // Purchase / Equip Status
    const char* statusText;
    Color statusColor;

    if (currentIndex == equippedIndex) {
        statusText = "EQUIPPED";
        statusColor = GREEN;
    } else if (current.isUnlocked) {
        statusText = "Press SPACE to Equip";
        statusColor = RAYWHITE;
    } else {
        statusText = TextFormat("Price: %d - Press SPACE to Buy", current.price);
        statusColor = (playerCoins >= current.price) ? YELLOW : RED;
    }

    DrawText(statusText, screenWidth / 2 - MeasureText(statusText, 30) / 2, screenHeight - 150, 30, statusColor);
    DrawText("Use LEFT/RIGHT to browse. Press ESC to return.", screenWidth / 2 - 250, screenHeight - 50, 20, LIGHTGRAY);
}

KiteProfile ShopManager::GetEquippedKite() {
    return kites[equippedIndex];
}

void ShopManager::Unload() {
    if (bgTexture.id > 0) UnloadTexture(bgTexture);
    
    for (auto& kite : kites) {
        if (kite.texture.id > 0) UnloadTexture(kite.texture);
    }
}