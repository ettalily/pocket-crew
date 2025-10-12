#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <algorithm>
#include <vector>

class Area {
    public:
    bool active = false;
    bool entitiesActive = false;
    const char *modelPath;
    Model model;
    BoundingBox modelBoundingBox;

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
    Vector3 position = (Vector3){ 0.0f, 5.0f, 0.0f },
    velocity = (Vector3){ 0.0f, 0.0f, 0.0f }, 
    direction = (Vector3){ 0.0f, 0.0f, 0.0f };
    BoundingBox playerColliderBox;
    float radius = 0.5f, acceleration = 0.02f, decceleration = 0.006f, maxVelocity = 0.17f, gravity = 0.01f, maxFallSpeed = 0.4f, jumpPower = 0.25f, wallJumpHorPower = 0.17f, wallSlideVelocity = 0.09f, jumpReleasePower = 0.045f, airAcceleration = 0.006f, airDecceleration = 0.0015, divePowerMult = 1.6f, diveMaxSpeed = 0.35f;
    unsigned int coyoteTimer = 0, wallCoyoteTimer = 0;
    bool touchingGround = false, jumpPressHeld = false, dived = false;
    void Update(); // player.cpp
    void UpdateMovementAxis(); // player.cpp
    void Move(); // player.cpp
    void Dive(); // player.cpp
    void Gravity(); // player.cpp
    void ApplyVelocity(); // player.cpp
    void JumpLogic(); // player.cpp
    void FloorDetect(RayCollision ray, Mesh mesh, Model model); // collision.cpp
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

// main.cpp
extern int gamepadID;
extern float dropShadowY;
extern Model dropShadow;
extern bool pause;

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