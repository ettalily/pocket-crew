#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <algorithm>
#include <vector>

class Collectable {
    public:
    Vector3 position, size = (Vector3){ 1.0f, 1.0f, 1.0f };
    BoundingBox hitbox;
    bool collected = false;

    void PickupCheck();
    void Draw();
};

class Area {
    public:
    bool active = false;
    bool entitiesActive = false;
    const char *modelPath;
    Model model;
    BoundingBox modelBoundingBox;
    void (*AreaLogic)();
    std::vector<Collectable> collectables;

    Vector3 position;
    float scale;

    void Load();
    void Draw();
};
extern Area firstIsland;

enum CameraSettings {
    Orbit
};
extern CameraSettings cameraMode;

class Player {
    public:
    Vector3 position = (Vector3){ 0.0f, 3.0f, 0.0f },
    velocity = Vector3Zero(), 
    direction = Vector3Zero();
    BoundingBox playerLogicBox, playerHitbox;
    float radius = 0.5f, acceleration = 0.018f, decceleration = 0.006f, maxVelocity = 0.17f, gravity = 0.01f, maxFallSpeed = 0.6f, jumpPower = 0.25f, wallJumpHorPower = 0.17f, wallSlideVelocity = 0.09f, jumpReleasePower = 0.045f, airAcceleration = 0.006f, airDecceleration = 0.0015, divePowerMult = 1.6f, diveMaxSpeed = 0.35f;
    unsigned int coyoteTimer = 0, wallCoyoteTimer = 0;
    bool touchingGround = false, jumpPressHeld = false, dived = false, touchingGroundAtStart = false;
    void Update(); // player.cpp
    void UpdateMovementAxis(); // player.cpp
    void Move(); // player.cpp
    void Dive(); // player.cpp
    void Gravity(); // player.cpp
    void ApplyVelocity(); // player.cpp
    void JumpLogic(); // player.cpp
    void FloorDetect(RayCollision ray); // collision.cpp
    void SlopeSteepness(Mesh mesh, Model model); // collision.cpp
    void SlopeStepDown(RayCollision front, RayCollision back); // collision.cpp
    void WallDetect(RayCollision ray, Vector3 dir); // collision.cpp
    void CollisionCheck(Mesh mesh, Model model); // collision.cpp
    void CeilingDetect(RayCollision ray); // collision.cpp
    void Collision(); // collision.cpp
};
extern Player player;

class GameCamera {
    public:
    Camera3D camera;
    Vector3 look, orbits;
    float offset = 20.0f;
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

    void DrawParticles();
};
extern Particle walkDust;

// main.cpp
extern int gamepadID;
extern float dropShadowY;
extern Model dropShadow;
extern bool pause;
extern Sound jumpSound;
extern Sound diveSound;

// camera.cpp
Vector3 GetForwardNormal();

// loading.cpp
extern Shader shader;
extern std::vector<Area*> loadedAreas;
void ShaderInit();
void UnloadDisabledAreas();

// player.cpp
extern float slopeMovementModifier;

// drawing.cpp
void Draw();

// Area Setup
void InitAreas();
void firstIslandInit();