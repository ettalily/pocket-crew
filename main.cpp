#include <iostream>
#include <raylib.h>
#include "raymath.h"

using namespace std;

const int screenWidth = 1600, screenHeight = 900; 
Camera3D camera;

struct Player {
    Vector3 position = (Vector3){ 0.0f, 0.0f, 0.0f }, 
    velocity = (Vector3){ 0.0f, 0.0f, 0.0f }, 
    direction = (Vector3){ 0.0f, 0.0f, 0.0f }, 
    size = (Vector3){ 1.0f, 1.0f, 1.0f };
    float acceleration = 0.03f, decceleration = 0.01f, maxVelocity = 0.15f;
};

Player player;

Vector3 GetForwardNormal() {
    return Vector3Normalize((Vector3){ camera.target.x, 0.0f, camera.target.z } - (Vector3){ camera.position.x, 0.0f, camera.position.z });
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

    // Applies velocity.
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

    while (!WindowShouldClose()) {
        PlayerMove();

        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        BeginMode3D(camera);
            DrawCube((Vector3){0.0f, -1.0f, 0.0f}, 10.0f, 1.0f, 10.0f, GREEN);
            DrawCubeWires((Vector3){0.0f, -1.0f, 0.0f}, 10.0f, 1.0f, 10.0f, BLACK);

            DrawCubeV(player.position, player.size, BLUE);
            DrawCubeWiresV(player.position, player.size, BLACK);
        EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}