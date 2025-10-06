#include "global.hpp"

const float stickDeadzone = 0.05f;
const int coyoteTimeLength = 6;
Vector2 dirInput;

Player player;

// Calls all the different parts of the player code.
void Player::Update() { 
    UpdateInputAxis();
    Move();
    Gravity();
    ApplyVelocity();
    Collision();
}

void Player::UpdateInputAxis() {
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
    if (Vector2Length(dirInput) >= 1) { if (Vector2Length((Vector2){ velocity.x, velocity.z }) < maxVelocity) { if (touchingGround) { velocity += moveVector * acceleration; } else { velocity += moveVector * airAcceleration; } }
    // Caps max velocity to scale with input magnitude. Applies it differently to the velocity depending on whether the player is touching the ground or not.
    } else if (Vector2Length((Vector2){ velocity.x, velocity.z }) < maxVelocity * Vector2Length(dirInput)) { if (touchingGround) { velocity += moveVector * acceleration; } else { velocity += moveVector * airAcceleration; } }

    // Applies drag/friction. Sets horizontal velocity to 0 if one application of drag would push the object past 0. Different amounts are applied depending on whether the player is touching the ground.
    if (touchingGround) {
        if (Vector2Length((Vector2){ velocity.x, velocity.z }) >= decceleration) { velocity -= Vector3Normalize((Vector3){ velocity.x, 0.0f, velocity.z }) * decceleration; } 
        else { velocity.x = 0.0f; velocity.z = 0.0f; }
    } 
    else if (Vector2Length((Vector2){ velocity.x, velocity.z }) >= airDecceleration) { velocity -= Vector3Normalize((Vector3){ velocity.x, 0.0f, velocity.z }) * airDecceleration; } 
    else { velocity.x = 0.0f; velocity.z = 0.0f; }

    // Sets the direction vector to the normalised horizontal direction.
    if (velocity.x != 0) { direction.x = velocity.x; }
    if (velocity.z != 0) { direction.z = velocity.z; }
    direction = Vector3Normalize(direction);

    // Jumping, wall sliding, and wall jumping.
    JumpLogic();
}

// Applies gravity within the max fall speed.
void Player::Gravity() {
    velocity.y -= gravity; 
    if (velocity.y <= -maxFallSpeed) { velocity.y = -maxFallSpeed; }
}

void Player::ApplyVelocity() {
    position += velocity;
}

void Player::JumpLogic() {
    // Jump hold difference.
    if ((IsKeyReleased(KEY_SPACE) || IsGamepadButtonReleased(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) && jumpPressHeld) { jumpPressHeld = false; if (velocity.y > 0) { velocity.y -= jumpReleasePower; } }

    // Sets and increments the kayote timer.
    if (touchingGround) { coyoteTimer = coyoteTimeLength; }
    else if (coyoteTimer > 0) { coyoteTimer -= 1; }

    // Sliding and jumping against a wall.
    if (!touchingGround && coyoteTimer == 0) {
        for (int m = 0; m < level.meshCount; m++) {
            // Raycasts in the direction of the stick input.
            RayCollision wallJumpCheckInputDir = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, (GetForwardNormal() * dirInput.y) + (Vector3Perpendicular(GetForwardNormal()) * dirInput.x) }, level.meshes[m], level.transform);
            RayCollision wallJumpCheckFacingDir = GetRayCollisionMesh(Ray{position, direction }, level.meshes[m], level.transform);
            // Wall slide.
            if ((wallJumpCheckInputDir.hit && abs(wallJumpCheckInputDir.normal.y) <= 0.2f && wallJumpCheckInputDir.distance <= radius + 0.1f) && (wallJumpCheckFacingDir.hit && abs(wallJumpCheckFacingDir.normal.y) <= 0.2f && wallJumpCheckFacingDir.distance <= radius + 0.1f)) { 
                if (velocity.y < -wallSlideVelocity) { velocity.y = -wallSlideVelocity; }
                // Wall jump.
                if ((IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))) {
                    velocity.y = jumpPower; jumpPressHeld = true; velocity = (Vector3){ wallJumpCheckInputDir.normal.x * wallJumpHorPower, velocity.y, wallJumpCheckInputDir.normal.z * wallJumpHorPower };
                }
                break;
            }
        }
    }
    // Jumping on the ground.
    else if ((touchingGround || coyoteTimer != 0) && (IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) ) {
        touchingGround = false; velocity.y = jumpPower; coyoteTimer = 0; jumpPressHeld = true;
    }
}