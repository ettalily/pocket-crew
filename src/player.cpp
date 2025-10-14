#include "global.hpp"

const float stickDeadzone = 0.05f;
const unsigned int coyoteTimeLength = 6;
const unsigned int wallCoyoteTimeLength = 10;
Vector2 dirInput;
float slopeMovementModifier = 1.0f;
Vector3 wallJumpDir;

Player player;

// Calls all the different parts of the player code.
void Player::Update() { 
    UpdateMovementAxis();
    Move();
    Gravity();
    ApplyVelocity();
    Collision();
    playerColliderBox = BoundingBox{position - (Vector3){ 5.0f, 5.0f, 5.0f }, position + (Vector3){ 5.0f, 5.0f, 5.0f }};
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

void Player::UpdateMovementAxis() {
    dirInput = Vector2Zero();
    // Checks if either left stick axis has passed the deadzone. If either has, it'll use the left stick input.
    if (abs(GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X)) >= stickDeadzone || abs(GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y)) >= stickDeadzone) {
        // Gets a normalised left stick input value that keeps it's magnitude.
        dirInput = (Vector2){ GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X), -GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y) } * Vector2Normalize((Vector2){ abs(GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X)), abs(GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y)) });
        // Applies deadzones.
        if (abs(GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X)) < stickDeadzone) { dirInput.x = 0.0f; }
        if (abs(GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y)) < stickDeadzone) { dirInput.y = 0.0f; }
    }
    // WASD/arrow key/d-pad input. Only runs if neither gamepad stick axis deadzone is passed, which includes when no controller is connected.
    else {
        // Gets the normalised keyboard/arrow key/d-pad input direction vector.
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP) || IsGamepadButtonDown(gamepadID, GAMEPAD_BUTTON_LEFT_FACE_UP)) { dirInput.y += 1.0f; }
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN) || IsGamepadButtonDown(gamepadID, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) { dirInput.y -= 1.0f; }
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT) || IsGamepadButtonDown(gamepadID, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) { dirInput.x += 1.0f; }
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT) || IsGamepadButtonDown(gamepadID, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) { dirInput.x -= 1.0f; }
        dirInput = Vector2Normalize(dirInput);
    }
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
}

void Player::JumpLogic() {
    // Jump hold difference.
    if ((IsGamepadButtonReleased(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsKeyReleased(KEY_K) || IsKeyReleased(KEY_H)) && jumpPressHeld && !dived) { jumpPressHeld = false; if (velocity.y > 0) { velocity.y -= jumpReleasePower; } }

    // Sets and increments the coyote timer.
    if (touchingGround) { coyoteTimer = coyoteTimeLength; wallCoyoteTimer = 0; }
    else if (coyoteTimer != 0) { coyoteTimer--; }
    // Increments the wall jump coyote timer.
    if (wallCoyoteTimer != 0) { wallCoyoteTimer--; }

    // Sliding and jumping against a wall.
    if (!touchingGround && coyoteTimer == 0) {
        for (auto it : loadedAreas) {
            // Checks if the player is inside the model bounding box.
            if (!CheckCollisionBoxes(player.playerColliderBox, it->modelBoundingBox)) {
                continue;
            }
            for (int m = 0; m < it->model.meshCount; m++) {
                // Checks whether the player is within each mesh's bounding box.
                if (!CheckCollisionBoxes(player.playerColliderBox, GetMeshBoundingBox(it->model.meshes[m]))) {
                    continue;
                }
                // Raycasts in the stick input direction and the player facing direction.
                RayCollision wallCheckInputDir = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, (GetForwardNormal() * dirInput.y) + (Vector3Perpendicular(GetForwardNormal()) * dirInput.x) }, it->model.meshes[m], it->model.transform);
                RayCollision wallCheckFacingDir = GetRayCollisionMesh(Ray{position, direction }, it->model.meshes[m], it->model.transform);
                if ((wallCheckInputDir.hit && abs(wallCheckInputDir.normal.y) <= 0.2f && wallCheckInputDir.distance <= radius + 0.2f) && (wallCheckFacingDir.hit && abs(wallCheckFacingDir.normal.y) <= 0.2f && wallCheckFacingDir.distance <= radius + 0.2f)) {
                    // Wall slide.
                    if (velocity.y < -wallSlideVelocity) { velocity.y = -wallSlideVelocity; }
                    // Allows the player to wall jump through the coyote timer and sets the direction the player would go horizontally from that wall.
                    if (wallCoyoteTimer == 0) { wallJumpDir = Vector3Normalize((Vector3){ wallCheckInputDir.normal.x, 0.0f, wallCheckInputDir.normal.z }); }  
                    wallCoyoteTimer = wallCoyoteTimeLength;
                    break;
                }
            }
        }
        // Wall jumping.
        if (wallCoyoteTimer != 0 && (IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsKeyPressed(KEY_K) || IsKeyPressed(KEY_H))) {
            velocity.y = jumpPower; jumpPressHeld = true; dived = false; wallCoyoteTimer = 0; velocity.x = wallJumpDir.x * wallJumpHorPower; velocity.z = wallJumpDir.z * wallJumpHorPower;
        }
    }
    // Jumping on the ground.
    else if ((touchingGround || coyoteTimer != 0) && (IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsKeyPressed(KEY_K) || IsKeyPressed(KEY_H)) ) {
        touchingGround = false; velocity.y = jumpPower; coyoteTimer = 0; jumpPressHeld = true;
    }
}

void Player::Dive() {
    if (!dived && !touchingGround && (IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_UP) || IsKeyPressed(KEY_J))) {
        dived = true;
        // Applies the dive values.
        velocity = (Vector3){ velocity.x * divePowerMult, velocity.y, velocity.z * divePowerMult };
        // Caps the max horizontal speed of a dive.
        if (Vector3Length((Vector3){ velocity.x, 0.0f, velocity.z }) > diveMaxSpeed) {
            velocity = (Vector3){ direction.x * diveMaxSpeed, velocity.y, direction.z * diveMaxSpeed };
        }       
    }
}