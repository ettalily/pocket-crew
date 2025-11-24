#include "global.hpp"

const int screenWidth = 1600, screenHeight = 900;
int gamepadID = 0;
bool pause = false;

float dropShadowY;
Model dropShadow;

Sound jumpSound, diveSound;

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    // Sets up the window and audio device.
    InitWindow(screenWidth, screenHeight, "Pocket Crew");
    SetTargetFPS(60);
    InitAudioDevice();

    // Sets up camera values.
    cam.CameraInit();
    
    // Loads the shaders.
    ShaderInit();

    // Load audio.
    Sound backgroundMusic = LoadSound("assets/music/background.mp3");
    jumpSound = LoadSound("assets/sound/jump.wav");
    diveSound = LoadSound("assets/sound/dive.wav");

    // Creates drop shadow.
    dropShadow = LoadModelFromMesh(GenMeshPlane(1.0f, 1.0f, 16, 16));
    dropShadow.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("assets/textures/dropshadow.png");

    // Setup level model values.
    InitAreas();

    // Load the starting area
    firstIsland.Load();

    // Game loop
    while (!WindowShouldClose()) {
        // Toggle borderless fullscreen.
        if (IsKeyPressed(KEY_EQUAL)) { ToggleBorderlessWindowed(); }

        if (IsKeyPressed(KEY_M)) { std::cout << std::to_string(player.position.x) + " " + std::to_string(player.position.y) + " " + std::to_string(player.position.z) << std::endl; }

        // Pause toggle.
        if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_MIDDLE_RIGHT)) { pause = !pause; }

        // Background music.
        if (!IsSoundPlaying(backgroundMusic)) { PlaySound(backgroundMusic); }
        
        if (!pause) {
            dropShadowY = -100.0f;
            player.Update();
            cam.Update();
        }

        // Update entities for loaded areas.
        for (auto it : loadedAreas) {
            it->AreaLogic();
        }

        // Updates the shader camera view vector.
        float cameraPos[3] = { cam.camera.position.x, cam.camera.position.y, cam.camera.position.z };
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

        // Draws the game.
        Draw();
    }

    // Unload all areas.
    for (auto it : loadedAreas) {
        it->active = false;
    }
    UnloadDisabledAreas();

    // Unloads other assets and resources.
    UnloadModel(dropShadow);
    UnloadSound(backgroundMusic);
    UnloadShader(shader);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}