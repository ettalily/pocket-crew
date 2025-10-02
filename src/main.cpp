#include "global.hpp"

const int screenWidth = 1600, screenHeight = 900;

Model level;

int main() {
    // Sets up the window.
    InitWindow(screenWidth, screenHeight, "Pocket Crew"); SetTargetFPS(60);

    // Sets up camera values.
    cam.CameraInit();

    level = LoadModel("assets/testmodel.glb");
    
    while (!WindowShouldClose()) {
        player.Update();
        cam.Update();

        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        BeginMode3D(cam.camera);
            DrawModel(level, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, GREEN);
            DrawModelWires(level, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, BLACK);
            DrawSphere(player.position, player.radius, BLUE);
        EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}