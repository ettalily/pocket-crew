#include "global.hpp"
#define RLIGHTS_IMPLEMENTATION
#include <rlights.h>
#define GLSL_VERSION 330

const int screenWidth = 1600, screenHeight = 900;

Model level;

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    // Sets up the window and audio device.
    InitWindow(screenWidth, screenHeight, "Pocket Crew"); SetTargetFPS(60);
    InitAudioDevice();

    // Load audio.
    Sound backgroundMusic = LoadSound("assets/music/background.mp3");

    // Sets up camera values.
    cam.CameraInit();

    // Loads the shader and sets it's values.
    Shader shader = LoadShader(TextFormat("resources/shaders/lighting.vs", GLSL_VERSION), TextFormat("resources/shaders/lighting.fs", GLSL_VERSION));
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);

    // Loads the level model and gives it the shader.
    level = LoadModel("assets/models/testmodel.glb");
    level.materials[0].shader = shader;
    
    // Create lights.
    Light light = CreateLight(LIGHT_POINT, (Vector3){ 50.0f, 100.0f, 50.0f }, Vector3Zero(), WHITE, shader);

    while (!WindowShouldClose()) {
        // Toggle borderless fullscreen.
        if (IsKeyPressed(KEY_EQUAL)) { ToggleBorderlessWindowed(); }

        // Background music.
        if (!IsSoundPlaying(backgroundMusic)) { PlaySound(backgroundMusic); }

        player.Update();
        cam.Update();

        // Updates the shader camera view vector.
        float cameraPos[3] = { cam.camera.position.x, cam.camera.position.y, cam.camera.position.z };
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

        // Drawing.
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
            BeginMode3D(cam.camera);
                BeginShaderMode(shader);
                    DrawModel(level, Vector3Zero(), 1.0f, WHITE);
                    DrawSphere(player.position, player.radius, WHITE);
                EndShaderMode();
            EndMode3D();
        DrawFPS(10, 10);
        EndDrawing();
    }

    // Unload assets and resources.
    UnloadShader(shader);
    UnloadModel(level);
    UnloadSound(backgroundMusic);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}