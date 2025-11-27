#include "global.hpp"

BugCollectable bugCollectables[1] = {
    *new BugCollectable{(Vector3){ 2.0f, 2.0f, 2.0f }}
};


void BugCollectable::BugUpdate() {
    
}

void BugCollectable::PickupCheck() {
    if (collected) {
        return;
    }
    if (CheckCollisionBoxes(player.playerHitbox, hitbox)) {
        collected = true;
        BugUpdate();
    }
}

void BugCollectable::Draw() {
    if (!collected) {
        DrawCube(position, size.x, size.y, size.z, GREEN);
    }
}