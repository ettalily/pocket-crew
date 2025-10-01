#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <rlights.h>

using namespace std;

const int screenWidth = 1600, screenHeight = 900;

const float movementDeadzone = 0.05f;
int gamepadID = 0;

class GameCamera {
    public:
    Camera3D camera;

    void CameraInit() {
        camera.projection = CAMERA_PERSPECTIVE;
        camera.fovy = 40.0f;
        camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
        camera.position = (Vector3){ 10.0f, 7.0f, 10.0f };
        camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    }
};
GameCamera cam;

Model level;

// Returns a normal that only considers horizontal directions. Used to figure out the vector for forward relative to the camera.
Vector3 GetForwardNormal() {
    return Vector3Normalize((Vector3){ cam.camera.target.x, 0.0f, cam.camera.target.z } - (Vector3){ cam.camera.position.x, 0.0f, cam.camera.position.z });
}

class Player {
    public:
    Vector3 position = (Vector3){ 0.0f, 0.0f, 0.0f },
    velocity = (Vector3){ 0.0f, 0.0f, 0.0f }, 
    direction = (Vector3){ 0.0f, 0.0f, 0.0f };
    float radius = 0.5f, acceleration = 0.02f, decceleration = 0.005f, maxVelocity = 0.12f, gravity = 0.01f, maxFallSpeed = 0.5f, jumpPower = 0.2f;
    bool touchingGround = false;
    
    // Calls all the different parts of the player code.
    void Update() {
        PlayerMove();
        Gravity();
        ApplyVelocity();
        Collision();
    }

    private:

    void FloorDetect(RayCollision ray) {
        if (velocity.y <= 0 && ray.hit && ray.distance <= radius && abs(ray.normal.y) >= 0.5f) {
            touchingGround = true; velocity.y = 0.0f; position.y = ray.point.y + (radius);
        }    
    }

    void WallDetect(RayCollision ray, Vector3 dir) {
        if (ray.hit && ray.distance < radius) {
            Vector3 horizontalOnlyNormal = Vector3Normalize((Vector3){ ray.normal.x, 0.0f, ray.normal.z });
            //velocity -= (Vector3){ abs(dir.x), 0.0f, abs(dir.z) } * velocity;
            position = (Vector3){ ray.point.x - (dir.x * radius) , position.y, ray.point.z - (dir.z * radius) };
        }   
    }

    // Handles wall, floor, and ceiling collisions. Very much so a WIP. I hate math and I have no idea how to properly structure this so it is purely trial and error at this point.
    void CollisionCheck(Mesh mesh, Model model) {
        // Floor Collision. Raycasts the center and all four corners of the base of the collision box.
        RayCollision basecenter = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
        FloorDetect(basecenter);
        RayCollision basedirposx = GetRayCollisionMesh(Ray{(Vector3){ position.x + (radius * 0.75f), position.y, position.z }, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
        FloorDetect(basedirposx);
        RayCollision basedirposz = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z + (radius * 0.75f) }, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
        FloorDetect(basedirposz);
        RayCollision basedirnegx = GetRayCollisionMesh(Ray{(Vector3){ position.x - (radius * 0.75f), position.y, position.z }, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
        FloorDetect(basedirnegx);
        RayCollision basedirnegz = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z - (radius * 0.75f) }, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
        FloorDetect(basedirnegz);

        // Wall Collision.
        RayCollision walldirforward = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, direction }, mesh, model.transform);
        RayCollision walldirleft = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, Vector3Perpendicular(direction) }, mesh, model.transform);
        RayCollision walldirright = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, Vector3Perpendicular(direction) * -1 }, mesh, model.transform);
        RayCollision walldirback = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, direction * -1 }, mesh, model.transform);
        
        WallDetect(walldirforward, direction);
        WallDetect(walldirleft, Vector3Perpendicular(direction));
        WallDetect(walldirright, Vector3Perpendicular(direction) * -1);
        WallDetect(walldirback, direction * -1);

        RayCollision walldirforwardleft = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, direction - Vector3Perpendicular(direction) }, mesh, model.transform);
        RayCollision walldirforwardright = GetRayCollisionMesh(Ray{(Vector3){ position.x, position.y, position.z }, direction + Vector3Perpendicular(direction) }, mesh, model.transform);
        WallDetect(walldirforwardleft, direction - Vector3Perpendicular(direction));
        WallDetect(walldirforwardright, direction + Vector3Perpendicular(direction));
    }

    void PlayerMove() {
        // Gets keyboard input direction vector
        Vector3 dirInput = { 0.0f, 0.0f, 0.0f };
        if (IsKeyDown(KEY_W)) { dirInput += GetForwardNormal(); }
        if (IsKeyDown(KEY_S)) { dirInput -= GetForwardNormal(); }
        if (IsKeyDown(KEY_D)) { dirInput += Vector3Perpendicular(GetForwardNormal()); }
        if (IsKeyDown(KEY_A)) { dirInput -= Vector3Perpendicular(GetForwardNormal()); }
        dirInput = Vector3Normalize(dirInput);

        // Left stick input value that is normalised but keeps it's magnitude.
        Vector2 leftStickInput = (Vector2){ GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X), -GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y) } * Vector2Normalize((Vector2){ abs(GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X)), abs(GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y)) });
        // Ignores the keyboard input if a controller input is detected.
        if (leftStickInput.x > movementDeadzone || leftStickInput.x < -movementDeadzone || leftStickInput.y > movementDeadzone || leftStickInput.y < -movementDeadzone) { dirInput = (Vector3){ 0.0f, 0.0f, 0.0f };  }
        // Gets gamepad input direction vector.
        if (leftStickInput.y > movementDeadzone || leftStickInput.y < -movementDeadzone) { dirInput += GetForwardNormal() * leftStickInput.y; }
        if (leftStickInput.x > movementDeadzone || leftStickInput.x < -movementDeadzone) { dirInput += Vector3Perpendicular(GetForwardNormal()) * leftStickInput.x; }

        // Applies input direction vector to the velocity.
        velocity += dirInput * acceleration;

        // Caps the max velocity.
        if (Vector2Length((Vector2){ velocity.x, velocity.z }) >= maxVelocity) { 
            Vector2 maxVolHorizontal = (Vector2Normalize((Vector2){ velocity.x, velocity.z }) * maxVelocity);
            velocity = (Vector3){ maxVolHorizontal.x, velocity.y, maxVolHorizontal.y };
        }

        // Applies drag/friction. Sets horizontal velocity to 0 if one application of drag would push the object past 0.
        if (Vector2Length((Vector2){ velocity.x, velocity.z }) >= decceleration) {
            velocity -= Vector3Normalize((Vector3){ velocity.x, 0.0f, velocity.z }) * decceleration;
        } 
        else { velocity.x = 0.0f; velocity.z = 0.0f; }

        // Sets the direction vector to the normalised horizontal direction.
        if (velocity.x != 0) { direction.x = velocity.x; }
        if (velocity.z != 0) { direction.z = velocity.z; }
        direction = Vector3Normalize(direction);

        // Jumping.
        if (touchingGround && (IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) ) {
            touchingGround = false; velocity.y = jumpPower;
        }
    }

    void Gravity() {
        // Applies gravity within the max fall speed.
        velocity.y -= gravity; 
        if (velocity.y <= -maxFallSpeed) { velocity.y = -maxFallSpeed; }
    }

    // Calls all collider objects.
    void Collision() {
        touchingGround = false;
        CollisionCheck(level.meshes[0], level);
    }

    void ApplyVelocity() {
        position += velocity;
    }
};
Player player;

int main() {
    // Sets up the window.
    InitWindow(screenWidth, screenHeight, "Pocket Crew"); SetTargetFPS(60);

    // Sets up camera values.
    cam.CameraInit();

    level = LoadModel("assets/testmodel.glb");
    
    while (!WindowShouldClose()) {
        player.Update();

        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        BeginMode3D(cam.camera);
            DrawModel(level, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, GREEN);
            DrawModelWires(level, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, BLACK);
            DrawSphere(player.position, player.radius, BLUE);
        EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}