#include "global.hpp"

#define LSL_PING_FREQUENCY 60

Sound deathSound;

Vector3 lastSafeLocation = (Vector3){ 0.0f, 3.0f, 0.0f };
unsigned int lslTimer= 0;

void OOBCheck() {
    if (player.position.y < -6) {
        player.position = lastSafeLocation;
        player.velocity = Vector3Zero();
        PlaySound(deathSound);
        return;
    }
    if (!player.touchingGround) {
        return;
    }
    lslTimer++;
    if (lslTimer == LSL_PING_FREQUENCY) {
        lslTimer = 0;
        lastSafeLocation = player.position;
        lastSafeLocation.y += 3.0f;
    }
}