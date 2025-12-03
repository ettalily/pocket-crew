#include "global.hpp"

#define BUG_JUMP_POWER 0.11f
#define BUG_JUMP_GRAVITY 0.007f

BugCollectable* currentBug;

BugCollectable bugCollectables[3] = {
    *new BugCollectable{(Vector3){ -1.989857f, 16.181393f, 37.484776f }, (Vector3){ 5.0f, 1.0f, -5.0f }, GREEN, 0.11f, 0.007f},
    *new BugCollectable{(Vector3){ -15.118676f, 92.578828f, 12.186043f }, (Vector3){ 1.0f, 1.0f, -7.0f }, YELLOW, 0.2f, 0.05f},
    *new BugCollectable{(Vector3){ -27.376028f, 131.887878f, 30.528135f }, (Vector3){ 2.0f, 1.0f, -6.0f }, DARKGREEN, 0.02f, 0.007f}
};

void BugCollectable::PickupCheck() {
    if (collected || pause || this != currentBug) return;
    if (!CheckCollisionBoxes(player.playerHitbox, hitbox)) return;

    pause = true;
    cam.cameraMode = Track;
    cam.camera.target = position + (Vector3){ 0.0f, 1.0f, 0.0f };
    cam.staticOffset = Vector3Normalize(cameraOffset) * offsetAmount;
    StartDialogue(&dialogue, &collected);
}

void BugCollectable::Draw() {
    if (collected) return;
    
    offsetY += velocity;
    if (offsetY <= 0.0f) {
        velocity = jumpPower;
        offsetY = 0.0f;
    } else {
        velocity -= gravity;
    }

    DrawCube((Vector3){ position.x, position.y + offsetY, position.z }, size.x, size.y, size.z, bugColor);
}