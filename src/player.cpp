#include "global.hpp"

#define COYOTE_TIME_DURATION 6
#define WALLJUMP_COYOTE_TIME_DURATION 10

#define WALK_DUST_PARTICLE_FREQUENCY 3
#define WALL_SLIDE_PARTICLE_FREQUENCY 12
#define WALK_DUST_REQUIRED_SPEED 0.05
#define WALK_SLIDE_CUTOFF_DUST 1
#define WALK_SLIDE_CUTOFF_BOUNCE 0.95

#define WALK_BOB_REQUIRED_SPEED 0.02
#define WALK_BOB_STRENGTH 0.03
#define WALK_BOB_DECLINE 0.0037

Vector3 wallJumpDir;
float slopeMovementModifier = 1.0f;

Sound jumpSound, diveSound, landSound, walkSound;

unsigned int dustKickUpTimer = 0;
unsigned int wallDustKickUpTimer = 0;
float walkBobOffsetVelocity = 0.0f;

Player player;

// Calls all the different parts of the player code.
void Player::Update() { 
    Move();
    Gravity();
    ApplyVelocity();
    Collision();
    playerLogicBox = BoundingBox{ position - (Vector3){ 8.0f, 8.0f, 8.0f }, position + (Vector3){ 8.0f, 8.0f, 8.0f } };
    playerHitbox = BoundingBox{ position - (Vector3){ radius * 0.5f, radius * 0.5f, radius * 0.5f }, position + (Vector3){ radius * 0.5f, radius * 0.5f, radius * 0.5f } };
    if (!touchingGroundAtStart && touchingGround && coyoteTimer == 0) { PlaySound(landSound); SpawnParticle(landDust); }
}

// Applies gravity within the max fall speed.
void Player::Gravity() {
    velocity.y -= gravity; 
    if (velocity.y <= -maxFallSpeed) { velocity.y = -maxFallSpeed; }
}

// Applies velocity vector to player position.
void Player::ApplyVelocity() {
    position += velocity;
}

void Player::Move() {
    // Gets gamepad input direction and modifies it by the direction that is forward from the camera's perspective.
    Vector3 moveVector = (GetForwardNormal() * dirInput.y) + (Vector3Perpendicular(GetForwardNormal()) * dirInput.x);
    // Caps max input magnitude at 1 if it exceeds that. Applies it differently to the velocity depending on whether the player is touching the ground or not.
    if (Vector2Length(dirInput) >= 1) { 
        if (Vector2Length((Vector2){ velocity.x, velocity.z }) < maxVelocity) { 
            if (touchingGround) { velocity += moveVector * acceleration * slopeMovementModifier; }
            else { velocity += moveVector * airAcceleration; } 
        }
    }
    // Max velocity scales with input magnitude. Applies it differently to the velocity depending on whether the player is touching the ground or not. 
    else { 
        if (Vector2Length((Vector2){ velocity.x, velocity.z }) < maxVelocity * Vector2Length(dirInput)) {
            if (touchingGround) { velocity += moveVector * acceleration * slopeMovementModifier; }
            else { velocity += moveVector * airAcceleration; }
        }
    }

    // Applies drag/friction. Sets horizontal velocity to 0 if one application of drag would push the object past 0. Different amounts are applied depending on whether the player is touching the ground.
    if (touchingGround) {
        if (Vector2Length((Vector2){ velocity.x, velocity.z }) >= decceleration) { velocity -= Vector3Normalize((Vector3){ velocity.x, 0.0f, velocity.z }) * decceleration; } 
        else { velocity.x = 0.0f; velocity.z = 0.0f; }
    } 
    else {
        if (Vector2Length((Vector2){ velocity.x, velocity.z }) >= airDecceleration) { velocity -= Vector3Normalize((Vector3){ velocity.x, 0.0f, velocity.z }) * airDecceleration; } 
        else { velocity.x = 0.0f; velocity.z = 0.0f; }
    }

    // Air diving.
    Dive();

    // Sets the direction vector to the normalised horizontal direction.
    if (velocity.x != 0) { direction.x = velocity.x; }
    if (velocity.z != 0) { direction.z = velocity.z; }
    direction = Vector3Normalize(direction);

    // Jumping, wall sliding, and wall jumping.
    JumpLogic();

    // Walk kick-up dust spawning and walk sound.
    if (touchingGround && Vector3Length(velocity) >= WALK_DUST_REQUIRED_SPEED) {
        dustKickUpTimer ++;
        if (Vector2Length((Vector2){ velocity.x, velocity.z } + (Vector2){ moveVector.x, moveVector.z }) >= WALK_SLIDE_CUTOFF_DUST) {
            if (dustKickUpTimer >= WALK_DUST_PARTICLE_FREQUENCY / Vector3Length(velocity)) {
                dustKickUpTimer = 0;
                PlaySound(walkSound);
                SpawnParticle(walkDust);
            }
        } else {
            if (dustKickUpTimer >= WALK_DUST_PARTICLE_FREQUENCY * 0.3f / Vector3Length(velocity)) {
                dustKickUpTimer = 0;
                PlaySound(walkSound);
                SpawnParticle(walkDust);
            }
        }
    } else {
        dustKickUpTimer = 0;
    }

    // Walk bobbing animation.
    if (Vector2Length(dirInput) > 0.0f && touchingGround && Vector3Length(velocity) >= WALK_BOB_REQUIRED_SPEED && Vector2Length((Vector2){ velocity.x, velocity.z } + (Vector2){ moveVector.x, moveVector.z }) >= WALK_SLIDE_CUTOFF_BOUNCE) {
        if (walkBobOffset == 0.0f) {
            walkBobOffsetVelocity = WALK_BOB_STRENGTH;
        } else {
            walkBobOffsetVelocity -= WALK_BOB_DECLINE;
        }
        walkBobOffset += walkBobOffsetVelocity;
        if (walkBobOffset <= 0.0f) {
            walkBobOffset = 0.0f;
        }
    } else {
        if (touchingGround && walkBobOffset > 0.0f) {
            walkBobOffsetVelocity -= WALK_BOB_DECLINE;
            walkBobOffset += walkBobOffsetVelocity;
        } else {
            walkBobOffset = 0.0f;
            walkBobOffsetVelocity = 0.0f;
        }
    }
}

void Player::JumpLogic() {
    // Jump hold difference.
    if ((IsGamepadButtonReleased(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsGamepadButtonReleased(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsKeyReleased(KEY_K) || IsKeyReleased(KEY_H)) && jumpPressHeld && !dived) { jumpPressHeld = false; if (velocity.y > 0) { velocity.y -= jumpReleasePower; } }

    // Sets and increments the coyote timer.
    if (touchingGround) { coyoteTimer = COYOTE_TIME_DURATION; wallCoyoteTimer = 0; }
    else if (coyoteTimer != 0) { coyoteTimer--; }
    // Increments the wall jump coyote timer.
    if (wallCoyoteTimer != 0) { wallCoyoteTimer--; }

    // Sliding and jumping against a wall.
    if (!touchingGround && coyoteTimer == 0) {
        for (auto i : loadedAreas) {
            // Checks if the player is inside the model bounding box.
            if (!CheckCollisionBoxes(playerLogicBox, i->modelBoundingBox)) {
                continue;
            }
            for (int m = 0; m < i->model.meshCount; m++) {
                // Checks whether the player is within each mesh's bounding box.
                if (!CheckCollisionBoxes(playerLogicBox, GetMeshBoundingBox(i->model.meshes[m]))) {
                    continue;
                }
                // Raycasts in the stick input direction and the player facing direction.
                RayCollision wallCheckInputDir = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, (GetForwardNormal() * dirInput.y) + (Vector3Perpendicular(GetForwardNormal()) * dirInput.x) }, i->model.meshes[m], i->model.transform);
                RayCollision wallCheckFacingDir = GetRayCollisionMesh(Ray{position, direction }, i->model.meshes[m], i->model.transform);
                if ((wallCheckInputDir.hit && abs(wallCheckInputDir.normal.y) <= 0.2f && wallCheckInputDir.distance <= radius + 0.2f) && (wallCheckFacingDir.hit && abs(wallCheckFacingDir.normal.y) <= 0.2f && wallCheckFacingDir.distance <= radius + 0.2f)) {
                    // Wall slide.
                    if (velocity.y < -wallSlideVelocity) { 
                        velocity.y = -wallSlideVelocity; 
                        wallDustKickUpTimer ++;
                        if (wallDustKickUpTimer >= WALL_SLIDE_PARTICLE_FREQUENCY) {
                            wallDustKickUpTimer = 0;
                            SpawnParticle(walkDust);
                        }
                    }
                    // Allows the player to wall jump through the coyote timer and sets the direction the player would go horizontally from that wall.
                    if (wallCoyoteTimer == 0) { wallJumpDir = Vector3Normalize((Vector3){ wallCheckInputDir.normal.x, 0.0f, wallCheckInputDir.normal.z }); }  
                    wallCoyoteTimer = WALLJUMP_COYOTE_TIME_DURATION;
                    break;
                }
            }
        }
        // Wall jumping.
        if (wallCoyoteTimer != 0 && (IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsKeyPressed(KEY_K) || IsKeyPressed(KEY_H))) {
            velocity.y = jumpPower;
            jumpPressHeld = true; 
            dived = false; 
            wallCoyoteTimer = 0; 
            velocity.x = wallJumpDir.x * wallJumpHorPower;
            velocity.z = wallJumpDir.z * wallJumpHorPower;
            PlaySound(jumpSound);
            SpawnParticle(walljumpDust);
        }
    }
    // Jumping on the ground.
    else if ((touchingGround || coyoteTimer != 0) && (IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsKeyPressed(KEY_K) || IsKeyPressed(KEY_H)) ) {
        touchingGround = false;
        velocity.y = jumpPower;
        coyoteTimer = 0; jumpPressHeld = true;
        PlaySound(jumpSound);
        SpawnParticle(jumpDust);
    }
}

void Player::Dive() {
    if (!dived && !touchingGround && (IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_UP) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_LEFT) || IsKeyPressed(KEY_J))) {
        dived = true;
        PlaySound(diveSound);
        SpawnParticle(diveDust);
        // Applies the dive values.
        velocity = (Vector3){ velocity.x * divePowerMult, velocity.y, velocity.z * divePowerMult };
        // Caps the max horizontal speed of a dive.
        if (Vector3Length((Vector3){ velocity.x, 0.0f, velocity.z }) > diveMaxSpeed) {
            velocity = (Vector3){ direction.x * diveMaxSpeed, velocity.y, direction.z * diveMaxSpeed };
        }       
    }
}