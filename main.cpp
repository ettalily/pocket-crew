#include <iostream>
#include <raylib.h>
#include "raymath.h"

using namespace std;

const int screenWidth = 1600, screenHeight = 900; 
Camera3D camera;

struct Player {
    Vector3 position = (Vector3){ 0.0f, 0.0f, 0.0f }, 
    startingPosition = (Vector3){ 0.0f, 0.0f, 0.0f },
    velocity = (Vector3){ 0.0f, 0.0f, 0.0f }, 
    direction = (Vector3){ 0.0f, 0.0f, 0.0f }, 
    size = (Vector3){ 1.0f, 1.0f, 1.0f };
    float acceleration = 0.03f, decceleration = 0.01f, maxVelocity = 0.15f, gravity = 0.01f, maxFallSpeed = 0.5f, jumpPower = 0.2f;
    bool touchingGround = false;
};

Player player;

class Collider {
    public:
    Vector3 position, size;
};

Collider ground;
Collider wall;

// Returns a normal that only considers horizontal directions. Used to figure out the vector for forward relative to the camera.
Vector3 GetForwardNormal() {
    return Vector3Normalize((Vector3){ camera.target.x, 0.0f, camera.target.z } - (Vector3){ camera.position.x, 0.0f, camera.position.z });
}

BoundingBox GetCubeBoundingBox(Vector3 position, Vector3 size) {
    return BoundingBox{position - (size/2), position + (size/2)};
}

// Handles wall, floor, and ceiling collisions.
void CubeCollisionCheck(Collider object) {
    if (CheckCollisionBoxes(GetCubeBoundingBox(player.position, player.size), GetCubeBoundingBox(object.position, object.size))) {
        if (player.position.x + (player.size.x/2) >= object.position.x - (object.size.x/2) && player.startingPosition.x - (player.size.x/2) <= object.position.x + (object.size.x/2) && player.startingPosition.z + (player.size.z/2) >= object.position.z - (object.size.z/2) && player.startingPosition.z - (player.size.z/2) <= object.position.z + (object.size.z/2)) {
            // +Y
            if (player.startingPosition.y + (player.size.y/2) <= object.position.y - (object.size.y/2)) {
                player.velocity.y = 0.0f; player.position.y = object.position.y - (object.size.y/2) - (player.size.y/2);
            }
            // -Y
            if (player.startingPosition.y - (player.size.y/2) >= object.position.y + (object.size.y/2)) {
                player.touchingGround = true; player.velocity.y = 0.0f; player.position.y = object.position.y + (object.size.y/2) + (player.size.y/2);
            }
        }
        if (player.position.y + (player.size.y/2) >= object.position.y - (object.size.y/2) && player.position.y - (player.size.y/2) <= object.position.y + (object.size.y/2)) {
            // +X
            if (player.startingPosition.x + (player.size.x/2) <= object.position.x - (object.size.x/2)) {
                player.velocity.x = 0.0f; player.position.x = object.position.x - (object.size.x/2) - (player.size.x/2);
            }
            // -X
            if (player.startingPosition.x - (player.size.x/2) >= object.position.x + (object.size.x/2)) {
                player.velocity.x = 0.0f; player.position.x = object.position.x + (object.size.x/2) + (player.size.x/2);
            }
            // +Z
            if (player.startingPosition.z + (player.size.z/2) <= object.position.z - (object.size.z/2)) {
                player.velocity.z = 0.0f; player.position.z = object.position.z - (object.size.z/2) - (player.size.z/2);
            }
            // -Z
            if (player.startingPosition.z - (player.size.z/2) >= object.position.z + (object.size.z/2)) {
                player.velocity.z = 0.0f; player.position.z = object.position.z + (object.size.z/2) + (player.size.z/2);
            }
        }
    }
}

void PlayerMove() {
    // Gets input direction vector
    Vector3 dirInput = { 0.0f, 0.0f, 0.0f };
    if (IsKeyDown(KEY_W)) { dirInput += GetForwardNormal(); }
    if (IsKeyDown(KEY_S)) { dirInput -= GetForwardNormal(); }
    if (IsKeyDown(KEY_D)) { dirInput += Vector3Perpendicular(GetForwardNormal()); }
    if (IsKeyDown(KEY_A)) { dirInput -= Vector3Perpendicular(GetForwardNormal()); }

    // Applies input direction vector to the velocity.
    player.velocity += Vector3Normalize(dirInput) * player.acceleration;

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

    if (player.touchingGround && IsKeyPressed(KEY_SPACE)) {
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
    CubeCollisionCheck(ground);
    CubeCollisionCheck(wall);
}

void ApplyVelocity() {
    player.position += player.velocity;
}

int main() {
    // Sets up the window.
    InitWindow(screenWidth, screenHeight, "Pocket"); SetTargetFPS(60);

    // Sets camera options.
    camera.projection = CAMERA_PERSPECTIVE;
    camera.fovy = 60.0f;
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.position = (Vector3){ 10.0f, 7.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };

    ground.position = (Vector3){ 0.0f, -1.0f, 0.0f }; ground.size = (Vector3){ 20.0f, 1.0f, 20.0f };
    wall.position = (Vector3){ 4.0f, 0.0f, 0.0f }; wall.size = (Vector3){ 2.0f, 2.0f, 2.0f };


    while (!WindowShouldClose()) {
        player.startingPosition = player.position;
        Gravity();
        PlayerMove();
        ApplyVelocity();
        Collision();

        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        BeginMode3D(camera);
            DrawCubeV(ground.position, ground.size, GREEN);
            DrawCubeWiresV(ground.position, ground.size, BLACK);
            DrawCubeV(wall.position, wall.size, GREEN);
            DrawCubeWiresV(wall.position, wall.size, BLACK);
            
            DrawCubeV(player.position, player.size, BLUE);
            DrawCubeWiresV(player.position, player.size, BLACK);
        EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}