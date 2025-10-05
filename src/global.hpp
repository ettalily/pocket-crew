#include <iostream>
#include <raylib.h>
#include <raymath.h>

// player.cpp
class Player {
    public:
    Vector3 position = (Vector3){ 0.0f, 5.0f, 0.0f },
    velocity = (Vector3){ 0.0f, 0.0f, 0.0f }, 
    direction = (Vector3){ 0.0f, 0.0f, 0.0f };
    float radius = 0.5f, acceleration = 0.015f, decceleration = 0.005f, maxVelocity = 0.16f, gravity = 0.01f, maxFallSpeed = 0.4f, jumpPower = 0.25f, wallJumpHorPower = 0.2f, wallSlideVelocity = 0.07f, jumpReleasePower = 0.045f, airAcceleration = 0.005f, airDecceleration = 0.002;
    bool touchingGround = false;
    void Update();
    void UpdateInputAxis();
    void Move();
    void Gravity();
    void ApplyVelocity();
    void JumpLogic();
    void FloorDetect(RayCollision ray); // collision.cpp
    void WallDetect(RayCollision ray, Vector3 dir); // collision.cpp
    void CollisionCheck(Mesh mesh, Model model); // collision.cpp
    void CeilingDetect(RayCollision ray); // collision.cpp
    void Collision(); // collision.cpp
};
extern Player player;

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