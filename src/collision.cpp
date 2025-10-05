#include "global.hpp"

// Calls all collider objects.
void Player::Collision() {
    touchingGround = false;
    for (int m = 0; m < level.meshCount; m++) {
        CollisionCheck(level.meshes[m], level);
    }
}

// Handles wall, floor, and ceiling collisions.
void Player::CollisionCheck(Mesh mesh, Model model) {
    // Floor Collision. Raycasts the center and all four corners of the base of the collision box.
    RayCollision basecenter = GetRayCollisionMesh(Ray{position, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basecenter);
    RayCollision basedirforward = GetRayCollisionMesh(Ray{player.position + (direction * radius * 0.75f), (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirforward);
    RayCollision basedirright = GetRayCollisionMesh(Ray{player.position + (Vector3Perpendicular(direction) * radius * 0.75f), (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirright);
    RayCollision basedirleft = GetRayCollisionMesh(Ray{player.position - (Vector3Perpendicular(direction) * radius * 0.75f), (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirleft);
    RayCollision basedirback = GetRayCollisionMesh(Ray{player.position - (direction * radius * 0.75f), (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirback);

    // Wall Collision. Raycasts in all four directions, with the direction of movement as forward.
    RayCollision walldirforward = GetRayCollisionMesh(Ray{position, direction }, mesh, model.transform);
    RayCollision walldirright = GetRayCollisionMesh(Ray{position, Vector3Perpendicular(direction) }, mesh, model.transform);
    RayCollision walldirleft = GetRayCollisionMesh(Ray{position, Vector3Perpendicular(direction) * -1 }, mesh, model.transform);
    RayCollision walldirback = GetRayCollisionMesh(Ray{position, direction * -1 }, mesh, model.transform);
    WallDetect(walldirforward, direction);
    WallDetect(walldirright, Vector3Perpendicular(direction));
    WallDetect(walldirleft, Vector3Perpendicular(direction) * -1);
    WallDetect(walldirback, direction * -1);

    // Ceiling Collision. Raycasts once straight above the center of the player.
    RayCollision topcenter = GetRayCollisionMesh(Ray{position, (Vector3){ 0.0f, 1.0f, 0.0f } }, mesh, model.transform);
    CeilingDetect(topcenter);
}

void Player::FloorDetect(RayCollision ray) {
    if (ray.hit) {
        if (velocity.y <= 0 && ray.distance <= radius && ray.normal.y >= 0.7f) {
            touchingGround = true; velocity.y = 0.0f; position.y = ray.point.y + (radius);
        }
        // Sets drop shadow height.
        if (ray.point.y > dropShadowY && ray.normal.y >= 0.7f) { dropShadowY = ray.point.y; }
    }
}

void Player::WallDetect(RayCollision ray, Vector3 dir) {
    if (ray.hit && ray.distance < radius) {
        Vector3 horizontalOnlyNormal = Vector3Normalize((Vector3){ ray.normal.x, 0.0f, ray.normal.z });
        position = (Vector3){ ray.point.x - (dir.x * radius) , position.y, ray.point.z - (dir.z * radius) };
        if (dir == direction) { velocity.x = 0.0f, velocity.z = 0.0f; }
    }
}

void Player::CeilingDetect(RayCollision ray) {
    if (ray.hit && ray.distance <= radius && ray.normal.y <= -0.7f) {
        velocity.y = 0.0f; position.y = ray.point.y - (radius);
    }
}