#include "global.hpp"

BugCollectable bugCollectables[1] = {
    *new BugCollectable{(Vector3){ -1.0f, 2.0f, 4.0f }, (Vector3){ 10.0f, 0.0f, 0.0f }}
};

void BugCollectable::BugUpdate() {
    
}

void BugCollectable::PickupCheck() {
    if (collected || pause) return;
    if (!CheckCollisionBoxes(player.playerHitbox, hitbox)) return;

    pause = true;
    cam.cameraMode = Track;
    cam.camera.target = position;
    cam.smoothing = true;
    cam.staticOffset = cameraOffset;
    StartDialogue(&dialogue, &collected);
}

void BugCollectable::Draw() {
    if (collected) return;
    
    DrawCube(position, size.x, size.y, size.z, GREEN);
}