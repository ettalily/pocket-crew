#include "global.hpp"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

bool pause = false;
bool pauseMenu = false;
bool closeGame = false;
bool musicOn, soundOn, showFps, devKeybinds;
Vector3 devPosition = Vector3Zero() + (Vector3){ 0.0f, 200.0f, 0.0f };

float dropShadowY;
Model dropShadow;

Sound backgroundMusic;

int main() {
    // Load settings preferences.
    InitCfg();
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    // Sets up the window and audio device.
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pocket Crew");
    SetTargetFPS(60);

    // Load audio.
    AudioInit();

    // Sets up camera values.
    cam.CameraInit();
    
    // Loads the shaders.
    ShaderInit();

    // Creates drop shadow.
    dropShadow = LoadModelFromMesh(GenMeshPlane(1.0f, 1.0f, 16, 16));
    dropShadow.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("assets/textures/dropshadow.png");

    // Loads particle textures and values.
    InitParticles();

    // Setup level model values.
    InitAreas();

    // Load dialogue text.
    DialogueInit();

    // Sets initial current bug value.
    CurrentBugUpdate();

    // Load the starting area
    firstIsland.Load();

    // Loads the bug sign and assigns it's initial value.
    InitSigns();

    // Game loop
    while (!WindowShouldClose() || closeGame) {

        // Developer Keybinds.
        if (devKeybinds && IsKeyPressed(KEY_M)) { std::cout << std::to_string(player.position.x) + "f, " + std::to_string(player.position.y) + "f, " + std::to_string(player.position.z) + "f " << "\n"; }
        if (devKeybinds && IsKeyPressed(KEY_PERIOD)) { player.position = devPosition; }
        if (devKeybinds && IsKeyPressed(KEY_COMMA)) { devPosition = player.position; }

        // Pause toggle.
        if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_MIDDLE_RIGHT)) { pauseMenu = !pauseMenu; }

        // Background music.
        if (musicOn && !IsSoundPlaying(backgroundMusic)) { PlaySound(backgroundMusic); }
        
        // Update controller inputs.
        UpdateMovementAxis();

        if (!pause && !pauseMenu) {
            dropShadowY = -100.0f;
            player.Update();
        }
        if (!(pauseMenu)) {
            cam.Update();
        }
        OOBCheck();

        // Update entities for loaded areas.
        for (auto i : loadedAreas) {
            i->AreaLogic();
        }

        // Updates dialogue.
        DialogueLogic();

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
    UnloadSound(dialogueEnterSound);
    UnloadSound(promptAcceptSound);
    UnloadSound(bugCollectSound);

    // Unloads other assets and resources.
    UnloadTexture(speechBubbleDialogue);
    UnloadTexture(bugSign);
    UnloadParticles();
    UnloadModel(dropShadow);
    UnloadShader(shader);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}