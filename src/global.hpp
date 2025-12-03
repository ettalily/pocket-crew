#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <raylib.h>
#include <raymath.h>
#include <fstream>
#include <filesystem>

#define PLAYER_RADIUS 0.5f
#define CAMERA_SPEED 0.35f

class Area {
    public:
    bool active = false;
    bool entitiesActive = false;
    const char *modelPath;
    Model model;
    BoundingBox modelBoundingBox;
    void (*AreaLogic)();

    Vector3 position;
    float scale;

    void Load();
    void Draw();
};
extern Area firstIsland;

enum CameraSettings {
    Orbit, Track
};

class Player {
    public:
    Vector3 position = (Vector3){ 0.0f, 3.0f, 0.0f },
    velocity = Vector3Zero(), 
    direction = Vector3Zero();
    BoundingBox playerLogicBox, playerHitbox;
    float walkBobOffset = 0.0f;
    unsigned int coyoteTimer = 0, wallCoyoteTimer = 0;
    bool touchingGround = false, jumpPressHeld = false, dived = false, touchingGroundAtStart = false;
    void Update(); // player.cpp
    void Move(); // player.cpp
    void Dive(); // player.cpp
    void Gravity(); // player.cpp
    void ApplyVelocity(); // player.cpp
    void JumpLogic(); // player.cpp
    void FloorDetect(RayCollision ray); // collision.cpp
    void SlopeSteepness(Mesh mesh, Model model); // collision.cpp
    void SlopeStepDown(RayCollision front, RayCollision back); // collision.cpp
    void WallDetect(RayCollision ray, Vector3 dir, Vector3 offset); // collision.cpp
    void CollisionCheck(Mesh mesh, Model model); // collision.cpp
    void CeilingDetect(RayCollision ray); // collision.cpp
    void Collision(); // collision.cpp
};
extern Player player;

class GameCamera {
    public:
    Camera3D camera;
    CameraSettings cameraMode = Orbit;
    Vector3 desiredPosition, orbits, lookDirection, staticOffset;
    bool smoothing = true;
    float offset = 20.0f, speed = CAMERA_SPEED;
    void CameraInit();
    void Update();
};
extern GameCamera cam;

class Particle {
    public:
    bool active = false;
    Vector3 position;
    Texture2D texture;
    int frameSize, frameCount, frameSpeed, currentFrame = 0, timer = 0;

    void Draw();
};
extern Particle walkDust, jumpDust, walljumpDust, diveDust, landDust;

class BugCollectable {
    public:
    Vector3 position, cameraOffset;
    Color bugColor;
    Vector3 size = (Vector3){ 0.4f, 0.4f, 0.4f };;
    std::vector<std::string> dialogue;
    bool collected = false;
    float offsetAmount = 10.0f;
    BoundingBox hitbox = BoundingBox{ position - (size * 3), position + (size * 3)};
    float offsetY = 0.0f, velocity = 0.0f;

    void PickupCheck();
    void Draw();
};

// preferences.cpp
void CfgUpdate(std::string file, std::string setting, std::string value);
void InitCfg();

// main.cpp
extern bool closeGame;
extern bool musicOn, soundOn, showFps, borderlessFullscreen;
extern float dropShadowY;
extern Model dropShadow;
extern bool pause, pauseMenu;
extern Sound backgroundMusic;

// camera.cpp
Vector3 GetForwardNormal();

// loading.cpp
extern Shader shader;
extern std::vector<Area*> loadedAreas;
void ShaderInit();
void AudioInit();
void DialogueInit();
void UnloadDisabledAreas();

// player.cpp
extern float slopeMovementModifier;
extern Sound jumpSound, diveSound, landSound, walkSound, slideSound;

// drawing.cpp
void DrawTextScreenScaled(Font font, const char *text, float posPercentX, float posPercentY, float sizePercent, float spacingPercent, float posAlignmentPercent, Color color);
void Draw();

// particles.cpp
void DrawParticles();
void InitParticles();
void SpawnParticle(Particle& particle);
void UnloadParticles();

// death.cpp
extern Sound deathSound;
void OOBCheck();

// input.cpp
extern int gamepadID;
extern Vector2 dirInput;
void UpdateMovementAxis();

// collectables.cpp
extern BugCollectable bugCollectables[1];

// buglocationsign.cpp
extern Texture2D bugSign;
void InitSigns();
void DrawBugSign();
void CurrentBugUpdate();

// dialogue.cpp
extern Sound dialogueEnterSound, promptAcceptSound, bugCollectSound;
extern bool inDialogue;
extern Texture2D speechBubbleDialogue;
void StartDialogue(std::vector<std::string>* dialogue, bool* collected);
void DialogueLogic();
void DrawDialogue();

// Area Setup
void InitAreas();
void FirstIslandInit();