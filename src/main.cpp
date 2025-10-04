#include "global.hpp"
#define RLIGHTS_IMPLEMENTATION
#include <rlights.h>
#define GLSL_VERSION 330

const int screenWidth = 1600, screenHeight = 900;
int gamepadID = 0;

Model level;
float dropShadowY;

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    // Sets up the window and audio device.
    InitWindow(screenWidth, screenHeight, "Pocket Crew"); SetTargetFPS(60);
    InitAudioDevice();

    // Sets up camera values.
    cam.CameraInit();

    // Loads the shader and sets it's values.
    Shader shader = LoadShader(TextFormat("resources/shaders/lighting.vs", GLSL_VERSION), TextFormat("resources/shaders/lighting.fs", GLSL_VERSION));
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);

    // Loads the level model and gives it the shader.
    level = LoadModel("assets/models/testmodel.glb");
    for (int m = 0; m < level.materialCount; m++) {
        level.materials[m].shader = shader;
    }

    // Load audio.
    Sound backgroundMusic = LoadSound("assets/music/background.mp3");
    
    // Create lights.
    Light light = CreateLight(LIGHT_DIRECTIONAL, (Vector3){ 50.0f, 100.0f, -50.0f }, Vector3Zero(), WHITE, shader);

    // Creates drop shadow.
    Model dropShadow = LoadModelFromMesh(GenMeshPlane(1.0f, 1.0f, 16, 16));
    dropShadow.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("assets/textures/dropshadow.png");

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
                    DrawPlane(Vector3Zero(), (Vector2){ 200.0f, 200.0f}, BLUE);
                    DrawSphere(player.position, player.radius, BLUE);
                    
                    // Draw player drop shadow.
                DrawModel(dropShadow, (Vector3){ player.position.x, dropShadowY + 0.05f, player.position.z }, 1.0f - ((player.position.y - dropShadowY) * 0.13f), WHITE);  
                EndShaderMode();
            EndMode3D();
        DrawFPS(10, 10);
        EndDrawing();
    }
    // Unload assets and resources.
    UnloadModel(dropShadow);
    UnloadModel(level);
    UnloadSound(backgroundMusic);
    UnloadShader(shader);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}