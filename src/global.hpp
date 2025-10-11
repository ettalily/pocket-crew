#include <iostream>
#include <raylib.h>
#include <raymath.h>

// player.cpp
class Player {
    public:
    Vector3 position = (Vector3){ 0.0f, 5.0f, 0.0f },
    velocity = (Vector3){ 0.0f, 0.0f, 0.0f }, 
    direction = (Vector3){ 0.0f, 0.0f, 0.0f };
    float radius = 0.5f, acceleration = 0.02f, decceleration = 0.006f, maxVelocity = 0.17f, gravity = 0.01f, maxFallSpeed = 0.4f, jumpPower = 0.25f, wallJumpHorPower = 0.17f, wallSlideVelocity = 0.09f, jumpReleasePower = 0.045f, airAcceleration = 0.006f, airDecceleration = 0.0015, divePowerMult = 1.6f, diveMaxSpeed = 0.35f;
    unsigned int coyoteTimer = 0, wallCoyoteTimer = 0;
    bool touchingGround = false, jumpPressHeld = false, dived = false;
    void Update();
    void UpdateMovementAxis();
    void Move();
    void Dive();
    void Gravity();
    void ApplyVelocity();
    void JumpLogic();
    void FloorDetect(RayCollision ray, Mesh mesh, Model model); // collision.cpp
    void WallDetect(RayCollision ray, Vector3 dir); // collision.cpp
    void CollisionCheck(Mesh mesh, Model model); // collision.cpp
    void CeilingDetect(RayCollision ray); // collision.cpp
    void Collision(); // collision.cpp
};
extern Player player;
extern float slopeMovementModifier;

// camera.cpp
enum CameraSettings {
    Orbit
};
extern CameraSettings cameraMode;

class GameCamera {
    public:
    Camera3D camera;
    Vector3 look, orbits;
    float offset = 20.0f;
    void CameraInit();
    void Update();
};
extern GameCamera cam;
Vector3 GetForwardNormal();

// main.cpp
extern int gamepadID;
extern Model level;
extern float dropShadowY;