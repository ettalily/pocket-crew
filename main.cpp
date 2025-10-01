#include <iostream>
#include <raylib.h>
#include "raymath.h"
#include "rlights.h"

using namespace std;

const int screenWidth = 1600, screenHeight = 900;
Camera3D camera;


const float movementDeadzone = 0.05f;
int gamepadID = 0;


struct Player {
    Vector3 position = (Vector3){ 0.0f, 0.0f, 0.0f },
    velocity = (Vector3){ 0.0f, 0.0f, 0.0f }, 
    direction = (Vector3){ 0.0f, 0.0f, 0.0f };
    float radius = 0.5f, acceleration = 0.02f, decceleration = 0.005f, maxVelocity = 0.12f, gravity = 0.01f, maxFallSpeed = 0.5f, jumpPower = 0.2f;
    bool touchingGround = false;
};

Player player;

Model groundTest;
Mesh groundMesh;

// Returns a normal that only considers horizontal directions. Used to figure out the vector for forward relative to the camera.
Vector3 GetForwardNormal() {
    return Vector3Normalize((Vector3){ camera.target.x, 0.0f, camera.target.z } - (Vector3){ camera.position.x, 0.0f, camera.position.z });
}

void FloorDetect(RayCollision ray) {
    if (player.velocity.y <= 0 && ray.hit && ray.distance <= player.radius) {
        player.touchingGround = true; player.velocity.y = 0.0f; player.position.y = ray.point.y + (player.radius);
    }    
}

void WallDetect(RayCollision ray, Vector3 dir) {
    if (ray.hit && ray.distance < player.radius) {
        Vector3 horizontalOnlyNormal = Vector3Normalize((Vector3){ ray.normal.x, 0.0f, ray.normal.z });
        //player.velocity -= (Vector3){ abs(dir.x), 0.0f, abs(dir.z) } * player.velocity;
        player.position = (Vector3){ ray.point.x - (dir.x * player.radius) , player.position.y, ray.point.z - (dir.z * player.radius) };
    }   
}

// Handles wall, floor, and ceiling collisions. Very much so a WIP. I hate math and I have no idea how to properly structure this so it is purely trial and error at this point.
void CollisionCheck(Mesh mesh, Model model) {
    // Floor Collision. Raycasts the center and all four corners of the base of the collision box.
    RayCollision basedirposx = GetRayCollisionMesh(Ray{(Vector3){ player.position.x + player.radius, player.position.y, player.position.z }, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirposx);
    RayCollision basedirposz = GetRayCollisionMesh(Ray{(Vector3){ player.position.x, player.position.y, player.position.z + player.radius }, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirposz);
    RayCollision basedirnegx = GetRayCollisionMesh(Ray{(Vector3){ player.position.x - player.radius, player.position.y, player.position.z }, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirnegx);
    RayCollision basedirnegz = GetRayCollisionMesh(Ray{(Vector3){ player.position.x, player.position.y, player.position.z - player.radius }, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirnegz);
    RayCollision basecenter = GetRayCollisionMesh(Ray{(Vector3){ player.position.x, player.position.y, player.position.z }, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basecenter);

    // Wall Collisionh.x * play
    RayCollision walldirforward = GetRayCollisionMesh(Ray{(Vector3){ player.position.x, player.position.y, player.position.z }, player.direction }, mesh, model.transform);
    RayCollision walldirleft = GetRayCollisionMesh(Ray{(Vector3){ player.position.x, player.position.y, player.position.z }, Vector3Perpendicular(player.direction) }, mesh, model.transform);
    RayCollision walldirright = GetRayCollisionMesh(Ray{(Vector3){ player.position.x, player.position.y, player.position.z }, Vector3Perpendicular(player.direction) * -1 }, mesh, model.transform);
    RayCollision walldirback = GetRayCollisionMesh(Ray{(Vector3){ player.position.x, player.position.y, player.position.z }, player.direction * -1 }, mesh, model.transform);
    
    WallDetect(walldirforward, player.direction);
    WallDetect(walldirleft, Vector3Perpendicular(player.direction));
    WallDetect(walldirright, Vector3Perpendicular(player.direction) * -1);
    WallDetect(walldirback, player.direction * -1);

    RayCollision walldirforwardleft = GetRayCollisionMesh(Ray{(Vector3){ player.position.x, player.position.y, player.position.z }, player.direction - Vector3Perpendicular(player.direction) }, mesh, model.transform);
    RayCollision walldirforwardright = GetRayCollisionMesh(Ray{(Vector3){ player.position.x, player.position.y, player.position.z }, player.direction + Vector3Perpendicular(player.direction) }, mesh, model.transform);
    WallDetect(walldirforwardleft, player.direction - Vector3Perpendicular(player.direction));
    WallDetect(walldirforwardright, player.direction + Vector3Perpendicular(player.direction));
}

void PlayerMove() {
    // Gets keyboard input direction vector
    Vector3 dirInput = { 0.0f, 0.0f, 0.0f };
    if (IsKeyDown(KEY_W)) { dirInput += GetForwardNormal(); }
    if (IsKeyDown(KEY_S)) { dirInput -= GetForwardNormal(); }
    if (IsKeyDown(KEY_D)) { dirInput += Vector3Perpendicular(GetForwardNormal()); }
    if (IsKeyDown(KEY_A)) { dirInput -= Vector3Perpendicular(GetForwardNormal()); }
    dirInput = Vector3Normalize(dirInput);

    // Left stick input value that is normalised but keeps it's magnitude.
    Vector2 leftStickInput = (Vector2){ GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X), -GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y) } * Vector2Normalize((Vector2){ abs(GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X)), abs(GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y)) });
    // Ignores the keyboard input if a controller input is detected.
    if (leftStickInput.x > movementDeadzone || leftStickInput.x < -movementDeadzone || leftStickInput.y > movementDeadzone || leftStickInput.y < -movementDeadzone) { dirInput = (Vector3){ 0.0f, 0.0f, 0.0f };  }
    // Gets gamepad input direction vector.
    if (leftStickInput.y > movementDeadzone || leftStickInput.y < -movementDeadzone) { dirInput += GetForwardNormal() * leftStickInput.y; }
    if (leftStickInput.x > movementDeadzone || leftStickInput.x < -movementDeadzone) { dirInput += Vector3Perpendicular(GetForwardNormal()) * leftStickInput.x; }

    // Applies input direction vector to the velocity.
    player.velocity += dirInput * player.acceleration;

    // Caps the max velocity.
    if (Vector2Length((Vector2){ player.velocity.x, player.velocity.z }) >= player.maxVelocity) { 
        Vector2 maxVolHorizontal = (Vector2Normalize((Vector2){ player.velocity.x, player.velocity.z }) * player.maxVelocity);
        player.velocity = (Vector3){ maxVolHorizontal.x, player.velocity.y, maxVolHorizontal.y };
    }

    // Applies drag/friction. Sets horizontal velocity to 0 if one application of drag would push the object past 0.
    if (Vector2Length((Vector2){ player.velocity.x, player.velocity.z }) >= player.decceleration) {
        player.velocity -= Vector3Normalize((Vector3){ player.velocity.x, 0.0f, player.velocity.z }) * player.decceleration;
    } 
    else { player.velocity.x = 0.0f; player.velocity.z = 0.0f; }

    // Sets the direction vector to the normalised horizontal direction.
    if (player.velocity.x != 0) { player.direction.x = player.velocity.x; }
    if (player.velocity.z != 0) { player.direction.z = player.velocity.z; }
    player.direction = Vector3Normalize(player.direction);

    // Jumping
    if (player.touchingGround && (IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) ) {
        player.touchingGround = false; player.velocity.y = player.jumpPower;
    }
}

void Gravity() {
    // Applies gravity within the max fall speed.
    player.velocity.y -= player.gravity; 
    if (player.velocity.y <= -player.maxFallSpeed) { player.velocity.y = -player.maxFallSpeed; }
}

// Calls all collider objects.
void Collision() {
    player.touchingGround = false;
    CollisionCheck(groundTest.meshes[0], groundTest);
}

void ApplyVelocity() {
    player.position += player.velocity;
}

int main() {
    // Sets up the window.
    InitWindow(screenWidth, screenHeight, "Pocket"); SetTargetFPS(60);

    groundTest = LoadModel("assets/testmodel.glb");

    // Sets camera options.
    camera.projection = CAMERA_PERSPECTIVE;
    camera.fovy = 40.0f;
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.position = (Vector3){ 10.0f, 7.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };

    while (!WindowShouldClose()) {
        PlayerMove();
        Gravity();
        ApplyVelocity();
        Collision();
        
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        BeginMode3D(camera);
            DrawModel(groundTest, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, GREEN);
            DrawModelWires(groundTest, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, BLACK);
            DrawSphere(player.position, player.radius, BLUE);
        EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}