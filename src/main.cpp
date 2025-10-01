#include "global.hpp"

const int screenWidth = 1600, screenHeight = 900;

class GameCamera {
    public:
    Camera3D camera;

    void CameraInit() {
        camera.projection = CAMERA_PERSPECTIVE;
        camera.fovy = 40.0f;
        camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
        camera.position = (Vector3){ 10.0f, 7.0f, 10.0f };
        camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    }
};
GameCamera cam;

Model level;

// Returns a normal that only considers horizontal directions. Used to figure out the vector for forward relative to the camera.
Vector3 GetForwardNormal() {
    return Vector3Normalize((Vector3){ cam.camera.target.x, 0.0f, cam.camera.target.z } - (Vector3){ cam.camera.position.x, 0.0f, cam.camera.position.z });
}

int main() {
    // Sets up the window.
    InitWindow(screenWidth, screenHeight, "Pocket Crew"); SetTargetFPS(60);

    // Sets up camera values.
    cam.CameraInit();

    level = LoadModel("assets/testmodel.glb");
    
    while (!WindowShouldClose()) {
        player.Update();

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