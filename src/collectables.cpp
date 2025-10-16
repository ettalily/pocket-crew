#include "global.hpp"

void Collectable::PickupCheck() {
    if (collected) {
        return;
    }
    hitbox = BoundingBox{ position - size, position + size };
    if (CheckCollisionBoxes(player.playerHitbox, hitbox)) {
        collected = true;
    }
}

void Collectable::Draw() {
    if (!collected) {
        DrawCube(position, size.x, size.y, size.z, RED);
    }
}