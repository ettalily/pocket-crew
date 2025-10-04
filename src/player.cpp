#include "global.hpp"

const float movementDeadzone = 0.05f;
const int coyoteTimeLength = 6;
int coyoteTimer = 0;
bool jumpPressHeld = false;

int gamepadID = 0;

Player player;

// Returns a normal that only considers horizontal directions. Used to figure out the vector for forward relative to the camera.
Vector3 GetForwardNormal() {
    return Vector3Normalize((Vector3){ cam.camera.target.x, 0.0f, cam.camera.target.z } - (Vector3){ cam.camera.position.x, 0.0f, cam.camera.position.z });
}

void Player::Update() { 
    // Calls all the different parts of the player code.
    dropShadowY = -100.0f;
    Move();
    Gravity();
    ApplyVelocity();
    Collision();
}

void Player::Move() {
    Vector3 dirInput = Vector3Zero();
    // Left stick input value that is normalised but keeps it's magnitude.
    Vector2 leftStickInput = (Vector2){ GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X), -GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y) } * Vector2Normalize((Vector2){ abs(GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X)), abs(GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y)) });

    // Uses the controller input if a stick direction is detected.
    if (leftStickInput.x > movementDeadzone || leftStickInput.x < -movementDeadzone || leftStickInput.y > movementDeadzone || leftStickInput.y < -movementDeadzone) { 
        // Gets gamepad input direction vector.
        if (leftStickInput.y > movementDeadzone || leftStickInput.y < -movementDeadzone) { dirInput += GetForwardNormal() * leftStickInput.y; }
        if (leftStickInput.x > movementDeadzone || leftStickInput.x < -movementDeadzone) { dirInput += Vector3Perpendicular(GetForwardNormal()) * leftStickInput.x; }
        if (Vector2Length((Vector2){ GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X), -GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y) }) >= 1) {
            if (Vector2Length((Vector2){ velocity.x, velocity.z }) < maxVelocity) { velocity += dirInput * acceleration; }
        } else if (Vector2Length((Vector2){ velocity.x, velocity.z }) < maxVelocity * Vector2Length((Vector2){ GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X), -GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y) })) { velocity += dirInput * acceleration; }
    }
    else
    {
        // Gets keyboard input direction vector
        if (IsKeyDown(KEY_W)) { dirInput += GetForwardNormal(); }
        if (IsKeyDown(KEY_S)) { dirInput -= GetForwardNormal(); }
        if (IsKeyDown(KEY_D)) { dirInput += Vector3Perpendicular(GetForwardNormal()); }
        if (IsKeyDown(KEY_A)) { dirInput -= Vector3Perpendicular(GetForwardNormal()); }
        if (Vector2Length((Vector2){ velocity.x, velocity.z }) < maxVelocity) { velocity += Vector3Normalize(dirInput) * acceleration; }
    }

    // Applies drag/friction. Sets horizontal velocity to 0 if one application of drag would push the object past 0.
    if (Vector2Length((Vector2){ velocity.x, velocity.z }) >= decceleration) {
        velocity -= Vector3Normalize((Vector3){ velocity.x, 0.0f, velocity.z }) * decceleration;
    } 
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

// Calls all collider objects.
void Player::Collision() {
    touchingGround = false;
    CollisionCheck(level.meshes[0], level);
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
    RayCollision wallcheck = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, direction }, level.meshes[0], level.transform);
    if (!touchingGround && wallcheck.hit && coyoteTimer == 0 && wallcheck.distance <= radius + 0.35f) { 
        if (velocity.y < -wallSlideVelocity) { velocity.y = -wallSlideVelocity; }
        if ((IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))) {
            velocity.y = jumpPower; jumpPressHeld = true; velocity = (Vector3){ wallcheck.normal.x * wallJumpHorPower, velocity.y, wallcheck.normal.z * wallJumpHorPower };
        }
    }
    // Jumping on the ground.
    else if ((touchingGround || coyoteTimer != 0) && (IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) ) {
        touchingGround = false; velocity.y = jumpPower; coyoteTimer = 0; jumpPressHeld = true;
    }
}