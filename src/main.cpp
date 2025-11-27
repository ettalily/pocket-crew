#include "global.hpp"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

bool pause = false;
bool closeGame = false;
bool musicOn = true;

float dropShadowY;
Model dropShadow;

Sound backgroundMusic;

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    // Sets up the window and audio device.
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pocket Crew");
    SetTargetFPS(60);
    InitAudioDevice();

    // Sets up camera values.
    cam.CameraInit();
    
    // Loads the shaders.
    ShaderInit();

    // Load audio.
    AudioInit();

    // Creates drop shadow.
    dropShadow = LoadModelFromMesh(GenMeshPlane(1.0f, 1.0f, 16, 16));
    dropShadow.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("assets/textures/dropshadow.png");

    // Loads particle textures and values.
    InitParticles();

    // Setup level model values.
    InitAreas();

    // Load settings preferences.
    InitCfg();

    // Load the starting area
    firstIsland.Load();

    // Game loop
    while (!WindowShouldClose() || closeGame) {
        // Toggle borderless fullscreen.
        if (IsKeyPressed(KEY_EQUAL)) { ToggleBorderlessWindowed(); }

        if (IsKeyPressed(KEY_M)) { std::cout << std::to_string(player.position.x) + " " + std::to_string(player.position.y) + " " + std::to_string(player.position.z) << std::endl; }

        // Pause toggle.
        if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_MIDDLE_RIGHT)) { pause = !pause; }

        // Background music.
        if (musicOn && !IsSoundPlaying(backgroundMusic)) { PlaySound(backgroundMusic); }
        
        // Update controller inputs
        UpdateMovementAxis();

        if (!pause) {
            dropShadowY = -100.0f;
            player.Update();
            cam.Update();
            OOBCheck();
        }

        // Update entities for loaded areas.
        for (auto i : loadedAreas) {
            i->AreaLogic();
        }

        // Updates the shader camera view vector.
        float cameraPos[3] = { cam.camera.position.x, cam.camera.position.y, cam.camera.position.z };
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

        // Draws the game.
        Draw();
    }

    // Unload all areas.
    for (auto i : loadedAreas) {
        i->active = false;
    }
    UnloadDisabledAreas();
    
    // Unloads sounds.
    UnloadSound(backgroundMusic);
    UnloadSound(jumpSound);
    UnloadSound(diveSound);
    UnloadSound(landSound);
    UnloadSound(walkSound);
    UnloadSound(deathSound);

    // Unloads other assets and resources.
    UnloadModel(dropShadow);
    UnloadShader(shader);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}