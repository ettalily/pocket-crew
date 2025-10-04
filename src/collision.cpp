#include "global.hpp"

// Calls all collider objects.
void Player::Collision() {
    touchingGround = false;
    for (int m = 0; m < level.meshCount; m++) {
        CollisionCheck(level.meshes[m], level);
    }
}

// Handles wall, floor, and ceiling collisions. Very much so a WIP. I hate math and I have no idea how to properly structure this so it is purely trial and error at this point.
void Player::CollisionCheck(Mesh mesh, Model model) {
    // Floor Collision. Raycasts the center and all four corners of the base of the collision box.
    RayCollision basecenter = GetRayCollisionMesh(Ray{position, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
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
    RayCollision walldirforward = GetRayCollisionMesh(Ray{position, direction }, mesh, model.transform);
    RayCollision walldirleft = GetRayCollisionMesh(Ray{position, Vector3Perpendicular(direction) }, mesh, model.transform);
    RayCollision walldirright = GetRayCollisionMesh(Ray{position, Vector3Perpendicular(direction) * -1 }, mesh, model.transform);
    RayCollision walldirback = GetRayCollisionMesh(Ray{position, direction * -1 }, mesh, model.transform);
    
    WallDetect(walldirforward, direction);
    WallDetect(walldirleft, Vector3Perpendicular(direction));
    WallDetect(walldirright, Vector3Perpendicular(direction) * -1);
    WallDetect(walldirback, direction * -1);

    RayCollision walldirforwardleft = GetRayCollisionMesh(Ray{position, direction - Vector3Perpendicular(direction) }, mesh, model.transform);
    RayCollision walldirforwardright = GetRayCollisionMesh(Ray{position, direction + Vector3Perpendicular(direction) }, mesh, model.transform);
    WallDetect(walldirforwardleft, direction - Vector3Perpendicular(direction));
    WallDetect(walldirforwardright, direction + Vector3Perpendicular(direction));
}

void Player::FloorDetect(RayCollision ray) {
    if (ray.hit) {
        if (velocity.y <= 0 && ray.distance <= radius && abs(ray.normal.y) >= 0.5f) {
            touchingGround = true; velocity.y = 0.0f; position.y = ray.point.y + (radius);
        }
        if (ray.point.y > dropShadowY) { dropShadowY = ray.point.y; }
    }
}

void Player::WallDetect(RayCollision ray, Vector3 dir) {
    if (ray.hit && ray.distance < radius) {
        Vector3 horizontalOnlyNormal = Vector3Normalize((Vector3){ ray.normal.x, 0.0f, ray.normal.z });
        position = (Vector3){ ray.point.x - (dir.x * radius) , position.y, ray.point.z - (dir.z * radius) };
        //velocity -= (Vector3){ abs(horizontalOnlyNormal.x), 0.0f, abs(horizontalOnlyNormal.z) } * velocity; Regressed until better solution.
    }   
}