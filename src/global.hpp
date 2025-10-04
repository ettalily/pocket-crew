#include <iostream>
#include <raylib.h>
#include <raymath.h>

// player.cpp
class Player {
    public:
    Vector3 position = (Vector3){ 0.0f, 2.0f, 0.0f },
    velocity = (Vector3){ 0.0f, 0.0f, 0.0f }, 
    direction = (Vector3){ 0.0f, 0.0f, 0.0f };
    float radius = 0.5f, acceleration = 0.015f, decceleration = 0.005f, maxVelocity = 0.14f, gravity = 0.01f, maxFallSpeed = 0.4f, jumpPower = 0.25f, wallJumpHorPower = 0.2f, wallSlideVelocity = 0.07f, jumpReleasePower = 0.045f, airDriftMult = 0.2f;
    bool touchingGround = false;
    void Update();
    void FloorDetect(RayCollision ray); // collision.cpp
    void WallDetect(RayCollision ray, Vector3 dir); // collision.cpp
    void CollisionCheck(Mesh mesh, Model model);
    void Move();
    void Gravity();
    void Collision(); // collision.cpp
    void ApplyVelocity();
    void JumpLogic();
};
extern Player player;

// camera.cpp
class GameCamera {
    public:
    Camera3D camera;
    Vector3 look, offset = (Vector3){ 10.0f, 7.0f, 10.0f };
    void CameraInit();
    void Update();
};
extern GameCamera cam;

// main.cpp
extern Model level;
extern float dropShadowY;