#include "global.hpp"

#define BUG_SIGN_SPEED 30
#define BUG_SIGN_HEIGHT_OFFSET 2.0f
#define BUG_SIGN_SIZE 2.0f

Texture2D bugSign;
int currentFrame = 1, frameTimer = 0;
bool enabled = true;

void InitSigns() {
    bugSign = LoadTexture("assets/textures/buglocationsign.png");
    CurrentBugUpdate();
}

void DrawBugSign() {
    if (pause || !enabled) return;
    DrawBillboardRec(cam.camera, bugSign, Rectangle{16.0f * currentFrame, 16.0f * currentFrame, 16.0f, 16.0f}, currentBug->position + (Vector3){ 0.0f, BUG_SIGN_HEIGHT_OFFSET, 0.0f }, (Vector2){ BUG_SIGN_SIZE, BUG_SIGN_SIZE }, WHITE);
    
    if (pauseMenu) return;
    frameTimer++;
    if (frameTimer != BUG_SIGN_SPEED) return;
    frameTimer = 0;
    currentFrame++;
    if (currentFrame > 3) {
        currentFrame = 1;
    }
}

void CurrentBugUpdate() {
    // Updates which bug the sign shows over.
    for (int i = 0; i < std::size(bugCollectables); i++) {
        if (bugCollectables[i].collected) continue;
        currentBug = &bugCollectables[i];
        return;
    }
    // Disables the sign if all bugs are collected.
    enabled = false;
}