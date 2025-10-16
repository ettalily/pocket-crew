#include "global.hpp"

const float floorNormalMin = 0.7f;
const float ceilingNormalMax = -0.7f;
const float stepDownBackDistance = 1.0f;
const float stepDownMaxDistanceDiff = 0.8f;
const float slopeSteepnessImpact = 0.65f;

void Player::Collision() {
    touchingGroundAtStart = touchingGround;
    touchingGround = false;
    // Iterates through each loaded area.
    for (auto it : loadedAreas) {
        // Checks if the player is inside the model bounding box.
        if (!CheckCollisionBoxes(player.playerColliderBox, it->modelBoundingBox)) {
            continue;
        }
        for (int m = 0; m < it->model.meshCount; m++) {
            // Checks whether the player is within each mesh's bounding box.
            if (CheckCollisionBoxes(player.playerColliderBox, GetMeshBoundingBox(it->model.meshes[m]))) {
                CollisionCheck(it->model.meshes[m], it->model);
            }
        }
    }
}

// Handles wall, floor, and ceiling collisions.
void Player::CollisionCheck(Mesh mesh, Model model) {
    // Floor Collision. Raycasts the center and all four corners of the base of the collision box.
    RayCollision basecenter = GetRayCollisionMesh(Ray{position, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basecenter);
    RayCollision basedirforward = GetRayCollisionMesh(Ray{player.position + (direction * radius * 0.75f), (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirforward);
    RayCollision basedirback = GetRayCollisionMesh(Ray{player.position - (direction * radius * 0.75f), (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirback);
    // Deals with stepping down slopes.
    SlopeStepDown(basedirforward, basedirback);
    RayCollision basedirright = GetRayCollisionMesh(Ray{player.position + (Vector3Perpendicular(direction) * radius * 0.75f), (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirright);
    RayCollision basedirleft = GetRayCollisionMesh(Ray{player.position - (Vector3Perpendicular(direction) * radius * 0.75f), (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirleft);
    
    // Changes the grounded movement speed based on the surface's steepness.
    SlopeSteepness(mesh, model);

    // Wall Collision. Raycasts in all four directions, with the direction of movement as forward.
    RayCollision walldirforward = GetRayCollisionMesh(Ray{position, direction }, mesh, model.transform);
    WallDetect(walldirforward, direction);
    RayCollision walldirright = GetRayCollisionMesh(Ray{position, Vector3Perpendicular(direction) }, mesh, model.transform);
    WallDetect(walldirright, Vector3Perpendicular(direction));
    RayCollision walldirleft = GetRayCollisionMesh(Ray{position, Vector3Perpendicular(direction) * -1 }, mesh, model.transform);
    WallDetect(walldirleft, Vector3Perpendicular(direction) * -1);
    RayCollision walldirback = GetRayCollisionMesh(Ray{position, direction * -1 }, mesh, model.transform);
    WallDetect(walldirback, direction * -1);

    // Ceiling Collision. Raycasts once straight above the center of the player.
    RayCollision topcenter = GetRayCollisionMesh(Ray{position, (Vector3){ 0.0f, 1.0f, 0.0f } }, mesh, model.transform);
    CeilingDetect(topcenter);
}

void Player::FloorDetect(RayCollision ray) {
    if (!ray.hit || ray.normal.y < floorNormalMin) {
        return;
    }
    if (velocity.y <= 0 && ray.distance <= radius) {
        touchingGround = true; dived = false; velocity.y = 0.0f; position.y = ray.point.y + radius;
    }
    if (ray.point.y > dropShadowY) { dropShadowY = ray.point.y; }
}

void Player::WallDetect(RayCollision ray, Vector3 dir) {
    if (ray.hit && ray.distance < radius) {
        position = (Vector3){ ray.point.x - (dir.x * radius) , position.y, ray.point.z - (dir.z * radius) };
        if (dir == direction) { velocity.x = 0.0f, velocity.z = 0.0f; }
    }
}

void Player::CeilingDetect(RayCollision ray) {
    if (ray.hit && ray.distance <= radius && ray.normal.y <= ceilingNormalMax) {
        velocity.y = 0.0f; position.y = ray.point.y - (radius);
    }
}

void Player::SlopeStepDown(RayCollision front, RayCollision back) {
    if (touchingGround || !touchingGroundAtStart || !front.hit || !back.hit) {
        return;
    }
    // Checks if the slope meets the slope step conditions.
    if (back.distance <= stepDownBackDistance && front.normal.y >= floorNormalMin && back.normal.y >= floorNormalMin && back.distance < front.distance && front.distance - back.distance <= stepDownMaxDistanceDiff) {
        touchingGround = true; dived = false; velocity.y = 0.0f; position.y = back.point.y + radius;
    }
}

void Player::SlopeSteepness(Mesh mesh, Model model) {
    if (!touchingGround) {
        return;
    }
    // Checks slope steepness and sets the slope steepness modifier.
    RayCollision slopefront = GetRayCollisionMesh(Ray{player.position + (direction * radius * 0.75f), (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    RayCollision slopeback = GetRayCollisionMesh(Ray{player.position - (direction * radius * 0.75f), (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    if (!slopefront.hit || !slopeback.hit) {
        return;
    }
    slopeMovementModifier = 1.0f + ((slopefront.distance - slopeback.distance) * slopeSteepnessImpact);
    // Caps the minimum and maximum slope movement modifier.
    if (slopeMovementModifier < 0.5f) { slopeMovementModifier = 0.5f; }
    if (slopeMovementModifier > 1.5f) { slopeMovementModifier = 1.5f; }
}