#include "global.hpp"

#define LSL_PING_FREQUENCY 120

Vector3 lastSafeLocation;
unsigned int lslTimer= 0;

void OOBCheck() {
    if (player.position.y < -10) {
        player.position = lastSafeLocation;
        return;
    }
    if (!player.touchingGround) {
        return;
    }
    lslTimer++;
    if (lslTimer == LSL_PING_FREQUENCY) {
        lslTimer = 0;
        lastSafeLocation = player.position;
    }
}