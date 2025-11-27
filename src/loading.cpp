#include "global.hpp"
#define RLIGHTS_IMPLEMENTATION
#include <rlights.h>
#define GLSL_VERSION 330

Shader shader;

std::vector<Area*> loadedAreas;

void ShaderInit() {
    // Loads the shader and sets it's values.
    shader = LoadShader(TextFormat("assets/shaders/lighting.vs", GLSL_VERSION), TextFormat("assets/shaders/lighting.fs", GLSL_VERSION));
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);

    // Create lights.
    Light light = CreateLight(LIGHT_DIRECTIONAL, (Vector3){ 50.0f, 100.0f, -50.0f }, Vector3Zero(), WHITE, shader);
}

void AudioInit() {
    backgroundMusic = LoadSound("assets/music/background.mp3");
    jumpSound = LoadSound("assets/sound/jump.wav");
    diveSound = LoadSound("assets/sound/dive.wav");
    landSound = LoadSound("assets/sound/land.wav");
    walkSound = LoadSound("assets/sound/walk.wav");
    deathSound = LoadSound("assets/sound/dies.wav");
    slideSound = LoadSound("assets/sound/slide.wav");
}

// Loads the level model and gives it the shader.
void Area::Load() {
    active = true;
    model = LoadModel(modelPath);
    modelBoundingBox = GetModelBoundingBox(model);
    for (int m = 0; m < model.materialCount; m++) {
        model.materials[m].shader = shader;
    }
    loadedAreas.push_back(this);
}

// Draws the model.
void Area::Draw() {
    DrawModel(model, position, scale, WHITE);
}

// Unloads the model for non active areas and removes it from the vector.
void UnloadDisabledAreas() {
    for (auto it = loadedAreas.begin(); it != loadedAreas.end();) {
        Area areaPtr = *loadedAreas[std::distance(loadedAreas.begin(), it)];
        if (!areaPtr.active) { UnloadModel(areaPtr.model); it = loadedAreas.erase(it); }
        else { ++it; }
    }
}

// Sets up the starting values for all the area objects on game start.
void InitAreas() {
    FirstIslandInit();
}