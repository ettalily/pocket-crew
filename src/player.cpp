#include "global.hpp"

#define COYOTE_TIME_DURATION 6
#define WALLJUMP_COYOTE_TIME_DURATION 10

#define WALK_DUST_PARTICLE_FREQUENCY 3
#define WALL_SLIDE_PARTICLE_FREQUENCY 12
#define WALK_DUST_REQUIRED_SPEED 0.05f
#define WALK_SLIDE_CUTOFF_DUST 1.0f
#define WALK_SLIDE_CUTOFF_BOUNCE 0.95f

#define WALK_BOB_REQUIRED_SPEED 0.02f
#define WALK_BOB_STRENGTH 0.03f
#define WALK_BOB_DECLINE 0.0037f

#define PLAYER_HITBOX_SCALE 0.2f
#define PLAYER_LOGIC_BOX_SIZE 8.0f

#define PLAYER_DIVE_VELOCITY_MIN 0.1f

#define PLAYER_GROUNDED_ACCELERATION 0.018f
#define PLAYER_GROUNDED_DECCELERATION 0.006f
#define PLAYER_MAX_VELOCITY 0.17f
#define PLAYER_GRAVITY 0.01f
#define PLAYER_MAX_FALL_SPEED 0.5f
#define PLAYER_JUMP_POWER 0.25f
#define PLAYER_WALL_JUMP_HOR_POWER 0.19f
#define PLAYER_WALL_SLIDE_VELOCITY 0.09f
#define PLAYER_JUMP_RELEASE_POWER 0.045f
#define PLAYER_AIR_ACCELERATION 0.006f
#define PLAYER_AIR_DECCELERATION 0.0015f
#define PLAYER_DIVE_MULTIPLIER 1.8f
#define PLAYER_DIVE_MAX_SPEED 0.35f 

Vector3 wallJumpDir;
float slopeMovementModifier = 1.0f;

Sound jumpSound, diveSound, landSound, walkSound, slideSound;

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
    playerLogicBox = BoundingBox{ position - (Vector3){ PLAYER_LOGIC_BOX_SIZE, PLAYER_LOGIC_BOX_SIZE, PLAYER_LOGIC_BOX_SIZE }, position + (Vector3){ PLAYER_LOGIC_BOX_SIZE, PLAYER_LOGIC_BOX_SIZE, PLAYER_LOGIC_BOX_SIZE } };
    playerHitbox = BoundingBox{ position - (Vector3){ PLAYER_RADIUS * PLAYER_HITBOX_SCALE, PLAYER_RADIUS * PLAYER_HITBOX_SCALE, PLAYER_RADIUS * PLAYER_HITBOX_SCALE }, position + (Vector3){ PLAYER_RADIUS * PLAYER_HITBOX_SCALE, PLAYER_RADIUS * PLAYER_HITBOX_SCALE, PLAYER_RADIUS * PLAYER_HITBOX_SCALE } };
    
    if (!touchingGroundAtStart && touchingGround && coyoteTimer == 0) {
        if (soundOn) PlaySound(landSound);
        SpawnParticle(landDust);
    }
}

// Applies gravity within the max fall speed.
void Player::Gravity() {
    velocity.y -= PLAYER_GRAVITY; 
    if (velocity.y <= -PLAYER_MAX_FALL_SPEED) {
        velocity.y = -PLAYER_MAX_FALL_SPEED;
    }
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
        if (Vector2Length((Vector2){ velocity.x, velocity.z }) < PLAYER_MAX_VELOCITY) { 
            if (touchingGround) {
                velocity += moveVector * PLAYER_GROUNDED_ACCELERATION * slopeMovementModifier;
            } else { 
                velocity += moveVector * PLAYER_AIR_ACCELERATION;
            } 
        }
    // Max velocity scales with input magnitude. Applies it differently to the velocity depending on whether the player is touching the ground or not.
    } else { 
        if (Vector2Length((Vector2){ velocity.x, velocity.z }) < PLAYER_MAX_VELOCITY * Vector2Length(dirInput)) {
            if (touchingGround) {
                velocity += moveVector * PLAYER_GROUNDED_ACCELERATION * slopeMovementModifier;
            } else {
                velocity += moveVector * PLAYER_AIR_ACCELERATION;
            }
        }
    }

    // Applies drag/friction. Sets horizontal velocity to 0 if one application of drag would push the object past 0. Different amounts are applied depending on whether the player is touching the ground.
    if (touchingGround) {
        if (Vector2Length((Vector2){ velocity.x, velocity.z }) >= PLAYER_GROUNDED_DECCELERATION) {
            velocity -= Vector3Normalize((Vector3){ velocity.x, 0.0f, velocity.z }) * PLAYER_GROUNDED_DECCELERATION;
        } else {
            velocity.x = 0.0f; velocity.z = 0.0f;
        }
    } else {
        if (Vector2Length((Vector2){ velocity.x, velocity.z }) >= PLAYER_AIR_DECCELERATION) {
            velocity -= Vector3Normalize((Vector3){ velocity.x, 0.0f, velocity.z }) * PLAYER_AIR_DECCELERATION;
        } else {
            velocity.x = 0.0f; velocity.z = 0.0f;
        }
    }

    // Air diving.
    Dive();

    // Sets the direction vector to the normalised horizontal direction.
    if (velocity.x != 0) {
        direction.x = velocity.x;
    }
    if (velocity.z != 0) {
        direction.z = velocity.z;
    }
    direction = Vector3Normalize(direction);

    // Jumping, wall sliding, and wall jumping.
    JumpLogic();

    // Walk kick-up dust spawning and walk sound.
    if (touchingGround && Vector3Length(velocity) >= WALK_DUST_REQUIRED_SPEED ) {
        dustKickUpTimer ++;
        if ((Vector3Length(velocity + moveVector) >= WALK_SLIDE_CUTOFF_DUST) || Vector3Length(velocity) < 0.155f) {
            if (dustKickUpTimer >= WALK_DUST_PARTICLE_FREQUENCY / Vector3Length(velocity)) {
                dustKickUpTimer = 0;
                if (soundOn) PlaySound(walkSound);
                SpawnParticle(walkDust);
            }
        } else {
            if (dustKickUpTimer >= WALK_DUST_PARTICLE_FREQUENCY * 0.3f / Vector3Length(velocity)) {
                dustKickUpTimer = 0;
                if (soundOn) PlaySound(walkSound);
                SpawnParticle(walkDust);
            }
        }
    } else {
        dustKickUpTimer = 0;
    }

    // Walk bobbing animation.
    if (Vector2Length(dirInput) > 0.0f && touchingGround && Vector3Length(velocity) >= WALK_BOB_REQUIRED_SPEED && wallCoyoteTimer == 0 && ((Vector3Length(velocity + moveVector) >= WALK_SLIDE_CUTOFF_BOUNCE) || (Vector3Length(moveVector) > 0.0f && Vector3Length(velocity) < 0.155f && Vector3Length(velocity) > 0.002f))) {
        walkBobOffset += walkBobOffsetVelocity;
        if (walkBobOffset <= 0.0f) {
            walkBobOffsetVelocity = WALK_BOB_STRENGTH * Vector2Length(dirInput);
            walkBobOffset = 0.0f;
        } else {
            walkBobOffsetVelocity -= WALK_BOB_DECLINE * Vector2Length(dirInput);
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
    if ((IsGamepadButtonReleased(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsGamepadButtonReleased(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsKeyReleased(KEY_K) || IsKeyReleased(KEY_H)) && jumpPressHeld && !dived) { jumpPressHeld = false; if (velocity.y > 0) { velocity.y -= PLAYER_JUMP_RELEASE_POWER; } }

    // Sets and increments the coyote timer.
    if (touchingGround) { 
        coyoteTimer = COYOTE_TIME_DURATION; 
        wallCoyoteTimer = 0; 
    } else if (coyoteTimer != 0) {
        coyoteTimer--;
    }
    // Increments the wall jump coyote timer.
    if (wallCoyoteTimer != 0) {
        wallCoyoteTimer--;
    }

    // Sliding and jumping against a wall.
    if (!touchingGround && coyoteTimer == 0) {
        for (auto i : loadedAreas) {
            // Checks if the player is inside the model bounding box.
            if (!CheckCollisionBoxes(playerLogicBox, i->modelBoundingBox)) continue;

            for (int m = 0; m < i->model.meshCount; m++) {
                // Checks whether the player is within each mesh's bounding box.
                if (!CheckCollisionBoxes(playerLogicBox, GetMeshBoundingBox(i->model.meshes[m]))) continue;

                // Raycasts in the stick input direction and the player facing direction.
                RayCollision wallCheckInputDir = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, (GetForwardNormal() * dirInput.y) + (Vector3Perpendicular(GetForwardNormal()) * dirInput.x) }, i->model.meshes[m], i->model.transform);
                RayCollision wallCheckFacingDir = GetRayCollisionMesh(Ray{position, direction }, i->model.meshes[m], i->model.transform);
                if ((wallCheckInputDir.hit && abs(wallCheckInputDir.normal.y) <= 0.2f && wallCheckInputDir.distance <= PLAYER_RADIUS + 0.2f) && (wallCheckFacingDir.hit && abs(wallCheckFacingDir.normal.y) <= 0.2f && wallCheckFacingDir.distance <= PLAYER_RADIUS + 0.2f)) {
                    // Wall slide.
                    if (velocity.y < -PLAYER_WALL_SLIDE_VELOCITY) { 
                        velocity.y = -PLAYER_WALL_SLIDE_VELOCITY; 
                        wallDustKickUpTimer ++;
                        dived = false;
                        if (wallDustKickUpTimer >= WALL_SLIDE_PARTICLE_FREQUENCY) {
                            wallDustKickUpTimer = 0;
                            SpawnParticle(walkDust);
                            if (soundOn) PlaySound(slideSound);
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
            velocity.y = PLAYER_JUMP_POWER;
            jumpPressHeld = true; 
            dived = false; 
            wallCoyoteTimer = 0; 
            velocity.x = wallJumpDir.x * PLAYER_WALL_JUMP_HOR_POWER;
            velocity.z = wallJumpDir.z * PLAYER_WALL_JUMP_HOR_POWER;
            if (soundOn) PlaySound(jumpSound);
            SpawnParticle(walljumpDust);
        }
    // Jumping on the ground.
    } else if ((touchingGround || coyoteTimer != 0) && (IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsKeyPressed(KEY_K) || IsKeyPressed(KEY_H)) ) {
        touchingGround = false;
        velocity.y = PLAYER_JUMP_POWER;
        coyoteTimer = 0; jumpPressHeld = true;
        if (soundOn) { PlaySound(jumpSound); }
        SpawnParticle(jumpDust);
    }
}

void Player::Dive() {
    if (!dived && !touchingGround && Vector3Length((Vector3){ velocity.x, 0.0f, velocity.z }) >= PLAYER_DIVE_VELOCITY_MIN && (IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_UP) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_LEFT) || IsKeyPressed(KEY_J))) {
        dived = true;
        if (soundOn) { PlaySound(diveSound); }
        SpawnParticle(diveDust);
        // Applies the dive values.
        velocity = (Vector3){ velocity.x * PLAYER_DIVE_MULTIPLIER, velocity.y, velocity.z * PLAYER_DIVE_MULTIPLIER };
        // Caps the max horizontal speed of a dive.
        if (Vector3Length((Vector3){ velocity.x, 0.0f, velocity.z }) > PLAYER_DIVE_MAX_SPEED) {
            velocity = (Vector3){ direction.x * PLAYER_DIVE_MAX_SPEED, velocity.y, direction.z * PLAYER_DIVE_MAX_SPEED };
        }       
    }
}