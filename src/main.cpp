#include "global.hpp"
#define RLIGHTS_IMPLEMENTATION
#include <rlights.h>
#define GLSL_VERSION 330

const int screenWidth = 1600, screenHeight = 900;

Model level;

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    // Sets up the window.
    InitWindow(screenWidth, screenHeight, "Pocket Crew"); SetTargetFPS(60);

    InitAudioDevice();

    Sound backgroundMusic = LoadSound("assets/music/background.mp3");

    level = LoadModel("assets/models/testmodel.glb");

    // Sets up camera values.
    cam.CameraInit();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_EQUAL)) { ToggleBorderlessWindowed(); }
        if (!IsSoundPlaying(backgroundMusic)) { PlaySound(backgroundMusic); }

        player.Update();
        cam.Update();

        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        BeginMode3D(cam.camera);
            DrawModel(level, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, WHITE);
            DrawModelWires(level, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, BLACK);
            DrawSphere(player.position, player.radius, BLUE);
        EndMode3D();
        DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadModel(level);
    UnloadSound(backgroundMusic);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}