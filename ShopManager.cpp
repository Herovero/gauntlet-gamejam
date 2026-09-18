#include "ShopManager.hpp"

ShopManager::ShopManager() {
    currentIndex = 0;
    equippedIndex = 0;

    // Define your unique kites and their traits here!
    kites.push_back({"Classic Wau", "assets/waubulan.png", 250.0f, 1.0f, 0, true});
    kites.push_back({"Wau Merak", "assets/waumerak.png", 320.0f, 1.2f, 150, false}); // Faster, but bigger hitbox
    kites.push_back({"Wau Kucing", "assets/waukucing.png", 220.0f, 0.8f, 300, false}); // Slower, but tiny hitbox
}

void ShopManager::Update(int& playerCoins, bool& returnToMenu) {
    // Navigation (Left / Right)
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        currentIndex++;
        if (currentIndex >= (int)kites.size()) currentIndex = 0;
    }
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        currentIndex--;
        if (currentIndex < 0) currentIndex = (int)kites.size() - 1;
    }

    // Buy or Equip (Space or Enter)
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        if (kites[currentIndex].isUnlocked) {
            equippedIndex = currentIndex; // Equip if already owned
        } else if (playerCoins >= kites[currentIndex].price) {
            // Buy if locked and affordable
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
    ClearBackground(DARKBLUE);
    KiteProfile current = kites[currentIndex];

    // Header
    DrawText("WAU SHOP", screenWidth / 2 - MeasureText("WAU SHOP", 40) / 2, 50, 40, GOLD);
    DrawText(TextFormat("Coins: %d", playerCoins), 50, 50, 30, YELLOW);

    // Kite Name and Stats
    DrawText(current.name.c_str(), screenWidth / 2 - MeasureText(current.name.c_str(), 50) / 2, 150, 50, WHITE);
    DrawText(TextFormat("Speed: %.0f", current.speed), screenWidth / 2 - 100, 250, 25, RAYWHITE);
    DrawText(TextFormat("Size: %.1fx", current.sizeMultiplier), screenWidth / 2 - 100, 290, 25, RAYWHITE);

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