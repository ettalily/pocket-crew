#include "global.hpp"

const float movementDeadzone = 0.05f;
const int coyoteTimeLength = 6;
int coyoteTimer = 0;

int gamepadID = 0;

Player player;

// Returns a normal that only considers horizontal directions. Used to figure out the vector for forward relative to the camera.
Vector3 GetForwardNormal() {
    return Vector3Normalize((Vector3){ cam.camera.target.x, 0.0f, cam.camera.target.z } - (Vector3){ cam.camera.position.x, 0.0f, cam.camera.position.z });
}

void Player::Update() { 
    // Calls all the different parts of the player code.
    Move();
    Gravity();
    ApplyVelocity();
    Collision();
}

void Player::FloorDetect(RayCollision ray) {
    if (velocity.y <= 0 && ray.hit && ray.distance <= radius && abs(ray.normal.y) >= 0.5f) {
        touchingGround = true; velocity.y = 0.0f; position.y = ray.point.y + (radius);
    }    
}

void Player::WallDetect(RayCollision ray, Vector3 dir) {
    if (ray.hit && ray.distance < radius) {
        Vector3 horizontalOnlyNormal = Vector3Normalize((Vector3){ ray.normal.x, 0.0f, ray.normal.z });
        //velocity -= (Vector3){ abs(dir.x), 0.0f, abs(dir.z) } * velocity;
        position = (Vector3){ ray.point.x - (dir.x * radius) , position.y, ray.point.z - (dir.z * radius) };
    }   
}

// Handles wall, floor, and ceiling collisions. Very much so a WIP. I hate math and I have no idea how to properly structure this so it is purely trial and error at this point.
void Player::CollisionCheck(Mesh mesh, Model model) {
    // Floor Collision. Raycasts the center and all four corners of the base of the collision box.
    RayCollision basecenter = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basecenter);
    RayCollision basedirposx = GetRayCollisionMesh(Ray{(Vector3){ position.x + (radius * 0.75f), position.y, position.z }, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirposx);
    RayCollision basedirposz = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z + (radius * 0.75f) }, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirposz);
    RayCollision basedirnegx = GetRayCollisionMesh(Ray{(Vector3){ position.x - (radius * 0.75f), position.y, position.z }, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirnegx);
    RayCollision basedirnegz = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z - (radius * 0.75f) }, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirnegz);

    // Wall Collision.
    RayCollision walldirforward = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, direction }, mesh, model.transform);
    RayCollision walldirleft = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, Vector3Perpendicular(direction) }, mesh, model.transform);
    RayCollision walldirright = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, Vector3Perpendicular(direction) * -1 }, mesh, model.transform);
    RayCollision walldirback = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, direction * -1 }, mesh, model.transform);
    
    WallDetect(walldirforward, direction);
    WallDetect(walldirleft, Vector3Perpendicular(direction));
    WallDetect(walldirright, Vector3Perpendicular(direction) * -1);
    WallDetect(walldirback, direction * -1);

    RayCollision walldirforwardleft = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, direction - Vector3Perpendicular(direction) }, mesh, model.transform);
    RayCollision walldirforwardright = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, direction + Vector3Perpendicular(direction) }, mesh, model.transform);
    WallDetect(walldirforwardleft, direction - Vector3Perpendicular(direction));
    WallDetect(walldirforwardright, direction + Vector3Perpendicular(direction));
}

void Player::Move() {
    Vector3 dirInput = { 0.0f, 0.0f, 0.0f };
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

    // Sets and increments the kayote timer.
    if (touchingGround) { coyoteTimer = coyoteTimeLength; }
    else if (coyoteTimer > 0) { coyoteTimer -= 1; }

    // Jumping and Wall Jumping.
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
    RayCollision wallcheck = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, direction }, level.meshes[0], level.transform);
    if (!touchingGround  && velocity.y < 0 && coyoteTimer == 0 && wallcheck.hit && wallcheck.distance <= radius + 0.1f) { 
        if (velocity.y < -wallSlideVelocity) { velocity.y = -wallSlideVelocity; }
        if ((IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))) {
            velocity.y = jumpPower; velocity = (Vector3){ wallcheck.normal.x * wallJumpHorPower, velocity.y, wallcheck.normal.z * wallJumpHorPower };
        }
    }
    else if ((touchingGround || coyoteTimer != 0) && (IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) ) {
        touchingGround = false; velocity.y = jumpPower;
    }
}