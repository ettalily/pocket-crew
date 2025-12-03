#include "global.hpp"

#define LSL_PING_FREQUENCY 60

Sound deathSound;

Vector3 lastSafeLocation = (Vector3){ 10.351066f, 3.706548f, 55.418526f };
unsigned int lslTimer= 0;

void OOBCheck() {
    // Move player to last safe grounded position if out of bounds.
    if (player.position.y < -6) {
        player.position = lastSafeLocation;
        player.velocity = Vector3Zero();
        cam.Update();
        cam.camera.position = cam.desiredPosition;
        PlaySound(deathSound);
        return;
    }

    // Update last safe grounded position.
    if (!player.touchingGround) return;
    lslTimer++;
    if (lslTimer == LSL_PING_FREQUENCY) {
        lslTimer = 0;
        lastSafeLocation = player.position;
        lastSafeLocation.y += 3.0f;
    }
}