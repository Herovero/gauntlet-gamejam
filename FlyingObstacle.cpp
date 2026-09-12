#include "FlyingObstacle.hpp"

FlyingObstacle::FlyingObstacle(int screenWidth, int screenHeight, bool flyLeft, std::vector<Texture2D> animFrames, float bgSpeed) {
    this->flyingLeft = flyLeft;
    this->frames = animFrames;
    this->speedY = bgSpeed;

    this->totalFrames = frames.size(); // Automatically detects frames
    this->currentFrame = 0;
    this->frameTimer = 0.0f;
    this->frameSpeed = 0.05f;
    this->animDirection = 1;

    rec.width = 120.0f;

    // Use the first frame to determine the hitbox size
    if (!frames.empty() && frames[0].id > 0) {
        float aspectRatio = (float)frames[0].height / (float)frames[0].width;
        rec.height = rec.width * aspectRatio;
    } else {
        rec.height = 60.0f;
    }

    rec.y = (float)GetRandomValue(50, screenHeight / 2);
    
    float baseSpeed = (float)GetRandomValue(160, 280);

    if (flyLeft) {
        rec.x = (float)screenWidth + 50.0f;
        speedX = -baseSpeed;
    } else {
        rec.x = -rec.width - 50.0f; 
        speedX = baseSpeed;
    }

    //GenTextureMipmaps(&texture);
    //SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);
}

void FlyingObstacle::Update(float dt) { 
    rec.x += speedX * dt; 
    rec.y += speedY * dt;

    // Ping-Pong Animation Logic
    frameTimer += dt;
    if (frameTimer >= frameSpeed) {
        frameTimer = 0.0f;
        currentFrame += animDirection;
        
        if (currentFrame >= totalFrames - 1) {
            currentFrame = totalFrames - 1; 
            animDirection = -1;             
        } 
        else if (currentFrame <= 0) {
            currentFrame = 0;               
            animDirection = 1;              
        }
    }
}

void FlyingObstacle::Draw() {
    if (!frames.empty() && frames[currentFrame].id > 0) {
        Texture2D currentTex = frames[currentFrame];
        
        // Flip the width if flying left
        float srcWidth = flyingLeft ? -(float)currentTex.width : (float)currentTex.width;
        
        Rectangle source = { 0.0f, 0.0f, srcWidth, (float)currentTex.height };
        DrawTexturePro(currentTex, source, rec, { 0, 0 }, 0.0f, WHITE);
    } else {
        DrawRectangleRec(rec, ORANGE);
    }
}

bool FlyingObstacle::IsOffScreen(int screenWidth, int /*screenHeight*/) {
    if (flyingLeft) return rec.x + rec.width < 0.0f;
    return rec.x > screenWidth;
}

void FlyingObstacle::SyncBackgroundSpeed(float bgSpeed) {
    this->speedY = bgSpeed;
}