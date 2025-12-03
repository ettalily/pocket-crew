#include "global.hpp"

#define BUG_JUMP_POWER 0.11f
#define BUG_JUMP_GRAVITY 0.007f

BugCollectable bugCollectables[2] = {
    *new BugCollectable{(Vector3){ -1.76f, 16.46f, 38.40f }, (Vector3){ 5.0f, 1.0f, -5.0f }, GREEN},
    *new BugCollectable{(Vector3){ -22.18f, 73.38f, 7.12f }, (Vector3){ 5.0f, 1.0f, 0.0f }, YELLOW}
};

void BugCollectable::PickupCheck() {
    if (collected || pause) return;
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
        velocity = BUG_JUMP_POWER;
        offsetY = 0.0f;
    } else {
        velocity -= BUG_JUMP_GRAVITY;
    }

    DrawCube((Vector3){ position.x, position.y + offsetY, position.z }, size.x, size.y, size.z, bugColor);
}