#include "ShopSystem.hpp"

GameState ShopSystem::Update(float dt, int screenWidth, Vector2 virtualMousePos,
                             ShopManager& shopManager, CoinManager& coinManager,
                             AudioManager& audio, WauBulan& wau, SwingingKid& kid,
                             float& currentStringLength) {
    bool returnToMenu = false;
    int coinsBefore = coinManager.totalCoins;
    
    // Process input and shop interactions
    shopManager.Update(coinManager.totalCoins, returnToMenu, dt, screenWidth, virtualMousePos);

    // Play purchase sound if balance decreased
    if (coinManager.totalCoins < coinsBefore) {
        audio.PlayPurchase();
    }
    
    // Handle exit and immediately apply the equipped kite
    if (returnToMenu) {
        KiteProfile equipped = shopManager.GetEquippedKite();
        
        wau.speed = equipped.speed;
        wau.radius = 25.0f * equipped.sizeMultiplier;
        wau.ChangeTexture(equipped.texturePath.c_str());
        
        currentStringLength = equipped.stringLength; 
        kid.stringLength = currentStringLength;
        
        return MENU;
    }

    return SHOP;
}