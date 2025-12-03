#include "global.hpp"

#define FLOOR_NORMAL_MIN 0.7
#define CEILING_NORMAL_MAX -0.7
#define STEP_DOWN_BACK_DISTANCE 1
#define STEP_DOWN_MAX_DISTANCE_DIFF 0.8
#define SLOPE_STEEPNESS_IMPACT 0.65

void Player::Collision() {
    touchingGroundAtStart = touchingGround;
    touchingGround = false;
    // Iterates through each loaded area.
    for (auto it : loadedAreas) {
        // Checks if the player is inside the model bounding box.
        if (!CheckCollisionBoxes(playerLogicBox, it->modelBoundingBox)) continue;
        for (int m = 0; m < it->model.meshCount; m++) {
            // Checks whether the player is within each mesh's bounding box.
            if (!CheckCollisionBoxes(playerLogicBox, GetMeshBoundingBox(it->model.meshes[m]))) continue;
            CollisionCheck(it->model.meshes[m], it->model);
        }
    }
}

// Handles floor, wall, and ceiling collisions.
void Player::CollisionCheck(Mesh mesh, Model model) {
    // Floor Collision. Raycasts the center and all four corners of the base of the collision box.
    RayCollision basecenter = GetRayCollisionMesh(Ray{ position, (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basecenter);
    RayCollision basedirforward = GetRayCollisionMesh(Ray{ position + (direction * PLAYER_RADIUS * 0.75f), (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirforward);
    RayCollision basedirback = GetRayCollisionMesh(Ray{ position - (direction * PLAYER_RADIUS * 0.75f), (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirback);
    // Deals with stepping down slopes.
    SlopeStepDown(basedirforward, basedirback);
    RayCollision basedirright = GetRayCollisionMesh(Ray{ position + ((Vector3){ -direction.z, direction.y, direction.x } * PLAYER_RADIUS * 0.75f), (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirright);
    RayCollision basedirleft = GetRayCollisionMesh(Ray{ position - ((Vector3){ -direction.z, direction.y, direction.x } * PLAYER_RADIUS * 0.75f), (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    FloorDetect(basedirleft);
    
    // Changes the grounded movement speed based on the surface's steepness.
    SlopeSteepness(mesh, model);

    // Front Wall Collision.
    RayCollision walldirforward = GetRayCollisionMesh(Ray{ position, direction }, mesh, model.transform);
    WallDetect(walldirforward, direction, Vector3Zero());
    RayCollision walldirforwardright = GetRayCollisionMesh(Ray{ position + ((Vector3){ -direction.z, direction.y, direction.x } * PLAYER_RADIUS * 0.75f), direction }, mesh, model.transform);
    WallDetect(walldirforwardright, direction, (Vector3){ -direction.z, direction.y, direction.x } * PLAYER_RADIUS * 0.75f);
    RayCollision walldirforwardleft = GetRayCollisionMesh(Ray{ position - ((Vector3){ -direction.z, direction.y, direction.x } * PLAYER_RADIUS * 0.75f), direction }, mesh, model.transform);
    WallDetect(walldirforwardleft, direction, (Vector3){ -direction.z, direction.y, direction.x } * PLAYER_RADIUS * -0.75f);
    RayCollision walldirforwardup = GetRayCollisionMesh(Ray{ position + (Vector3){ 0.0f, PLAYER_RADIUS * 0.75f, 0.0f }, direction }, mesh, model.transform);
    WallDetect(walldirforwardup, direction, Vector3Zero());
    RayCollision walldirforwarddown = GetRayCollisionMesh(Ray{ position - (Vector3){ 0.0f, PLAYER_RADIUS * 0.75f, 0.0f }, direction }, mesh, model.transform);
    WallDetect(walldirforwarddown, direction, Vector3Zero());

    // Right Wall Collision.
    RayCollision walldirright = GetRayCollisionMesh(Ray{ position, (Vector3){ -direction.z, direction.y, direction.x } }, mesh, model.transform);
    WallDetect(walldirright, (Vector3){ -direction.z, direction.y, direction.x }, Vector3Zero());
    RayCollision walldirrightfront = GetRayCollisionMesh(Ray{ position + (direction * PLAYER_RADIUS * 0.75f), (Vector3){ -direction.z, direction.y, direction.x } }, mesh, model.transform);
    WallDetect(walldirrightfront, (Vector3){ -direction.z, direction.y, direction.x }, (direction * PLAYER_RADIUS * 0.75f));
    RayCollision walldirrightback = GetRayCollisionMesh(Ray{ position - (direction * PLAYER_RADIUS * 0.75f), (Vector3){ -direction.z, direction.y, direction.x } }, mesh, model.transform);
    WallDetect(walldirrightback, (Vector3){ -direction.z, direction.y, direction.x }, (direction * PLAYER_RADIUS * -0.75f));
    RayCollision walldirrightup = GetRayCollisionMesh(Ray{ position + (Vector3){ 0.0f, PLAYER_RADIUS * 0.75f, 0.0f }, (Vector3){ -direction.z, direction.y, direction.x } }, mesh, model.transform);
    WallDetect(walldirrightup, (Vector3){ -direction.z, direction.y, direction.x }, Vector3Zero());
    RayCollision walldirrightdown = GetRayCollisionMesh(Ray{ position - (Vector3){ 0.0f, PLAYER_RADIUS * 0.75f, 0.0f }, (Vector3){ -direction.z, direction.y, direction.x } }, mesh, model.transform);
    WallDetect(walldirrightdown, (Vector3){ -direction.z, direction.y, direction.x }, Vector3Zero());
    
    // Left Wall Collision.
    RayCollision walldirleft = GetRayCollisionMesh(Ray{ position, (Vector3){ -direction.z, direction.y, direction.x } * -1 }, mesh, model.transform);
    WallDetect(walldirleft, (Vector3){ -direction.z, direction.y, direction.x } * -1, Vector3Zero());
    RayCollision walldirleftfront = GetRayCollisionMesh(Ray{ position + (direction * PLAYER_RADIUS * 0.75f), (Vector3){ -direction.z, direction.y, direction.x } * -1 }, mesh, model.transform);
    WallDetect(walldirleftfront, (Vector3){ -direction.z, direction.y, direction.x } * -1, (direction * PLAYER_RADIUS * 0.75f));
    RayCollision walldirleftback = GetRayCollisionMesh(Ray{ position - (direction * PLAYER_RADIUS * 0.75f), (Vector3){ -direction.z, direction.y, direction.x } * -1 }, mesh, model.transform);
    WallDetect(walldirleftback, (Vector3){ -direction.z, direction.y, direction.x } * -1, (direction * PLAYER_RADIUS * -0.75f));
    RayCollision walldirleftup = GetRayCollisionMesh(Ray{ position + (Vector3){ 0.0f, PLAYER_RADIUS * 0.75f, 0.0f }, (Vector3){ -direction.z, direction.y, direction.x } * -1 }, mesh, model.transform);
    WallDetect(walldirleftup, (Vector3){ -direction.z, direction.y, direction.x } * -1, Vector3Zero());
    RayCollision walldirleftdown = GetRayCollisionMesh(Ray{ position - (Vector3){ 0.0f, PLAYER_RADIUS * 0.75f, 0.0f }, (Vector3){ -direction.z, direction.y, direction.x } * -1 }, mesh, model.transform);
    WallDetect(walldirleftdown, (Vector3){ -direction.z, direction.y, direction.x } * -1, Vector3Zero());
    
    // Back Wall Collision.
    RayCollision walldirback = GetRayCollisionMesh(Ray{ position, direction * -1 }, mesh, model.transform);
    WallDetect(walldirback, direction * -1, Vector3Zero());
    RayCollision walldirbackright = GetRayCollisionMesh(Ray{ position + ((Vector3){ -direction.z, direction.y, direction.x } * PLAYER_RADIUS * 0.75f), direction * -1 }, mesh, model.transform);
    WallDetect(walldirbackright, direction * -1, (Vector3){ -direction.z, direction.y, direction.x } * PLAYER_RADIUS * 0.75f);
    RayCollision walldirbackleft = GetRayCollisionMesh(Ray{ position - ((Vector3){ -direction.z, direction.y, direction.x } * PLAYER_RADIUS * 0.75f), direction * -1 }, mesh, model.transform);
    WallDetect(walldirbackleft, direction * -1, (Vector3){ -direction.z, direction.y, direction.x } * PLAYER_RADIUS * -0.75f);
    RayCollision walldirbackup = GetRayCollisionMesh(Ray{ position + (Vector3){ 0.0f, PLAYER_RADIUS * 0.75f, 0.0f }, direction * -1 }, mesh, model.transform);
    WallDetect(walldirbackup, direction * -1, Vector3Zero());
    RayCollision walldirbackdown = GetRayCollisionMesh(Ray{ position - (Vector3){ 0.0f, PLAYER_RADIUS * 0.75f, 0.0f }, direction * -1 }, mesh, model.transform);
    WallDetect(walldirbackdown, direction * -1, Vector3Zero());

    // Ceiling Collision. Raycasts once straight above the center of the player.
    RayCollision topcenter = GetRayCollisionMesh(Ray{ position, (Vector3){ 0.0f, 1.0f, 0.0f } }, mesh, model.transform);
    CeilingDetect(topcenter);
    RayCollision topdirforward = GetRayCollisionMesh(Ray{ position + (direction * PLAYER_RADIUS * 0.75f), (Vector3){ 0.0f, 1.0f, 0.0f } }, mesh, model.transform);
    CeilingDetect(topdirforward);
    RayCollision topdirback = GetRayCollisionMesh(Ray{ position - (direction * PLAYER_RADIUS * 0.75f), (Vector3){ 0.0f, 1.0f, 0.0f } }, mesh, model.transform);
    CeilingDetect(topdirback);
    RayCollision topdirright = GetRayCollisionMesh(Ray{ position + ((Vector3){ -direction.z, direction.y, direction.x } * PLAYER_RADIUS * 0.75f), (Vector3){ 0.0f, 1.0f, 0.0f } }, mesh, model.transform);
    CeilingDetect(topdirright);
    RayCollision topdirleft = GetRayCollisionMesh(Ray{ position - ((Vector3){ -direction.z, direction.y, direction.x } * PLAYER_RADIUS * 0.75f), (Vector3){ 0.0f, 1.0f, 0.0f } }, mesh, model.transform);
    CeilingDetect(topdirleft);
}

void Player::FloorDetect(RayCollision ray) {
    if (!ray.hit || ray.normal.y < FLOOR_NORMAL_MIN) return;

    if (velocity.y <= 0 && ray.distance <= PLAYER_RADIUS) {
        touchingGround = true; dived = false; velocity.y = 0.0f; position.y = ray.point.y + PLAYER_RADIUS;
    }

    if (ray.point.y > dropShadowY) { 
        dropShadowY = ray.point.y;
    }
}

void Player::WallDetect(RayCollision ray, Vector3 dir, Vector3 offset) {
    if (!ray.hit || ray.distance >= PLAYER_RADIUS) return;

    position = (Vector3){ (ray.point.x - (dir.x * PLAYER_RADIUS)) - offset.x , position.y, ray.point.z - ((dir.z * PLAYER_RADIUS)) - offset.z };   
    if (dir == direction) { velocity.x = 0.0f, velocity.z = 0.0f; }
}

void Player::CeilingDetect(RayCollision ray) {
    if (ray.hit && ray.distance <= PLAYER_RADIUS && ray.normal.y <= CEILING_NORMAL_MAX) {
        velocity.y = 0.0f; position.y = ray.point.y - (PLAYER_RADIUS);
    }
}

void Player::SlopeStepDown(RayCollision front, RayCollision back) {
    if (touchingGround || !touchingGroundAtStart || !front.hit || !back.hit) return;

    // Checks if the slope meets the slope step conditions.
    if (back.distance <= STEP_DOWN_BACK_DISTANCE && front.normal.y >= FLOOR_NORMAL_MIN && back.normal.y >= FLOOR_NORMAL_MIN && back.distance < front.distance && front.distance - back.distance <= STEP_DOWN_MAX_DISTANCE_DIFF) {
        touchingGround = true; dived = false; velocity.y = 0.0f; position.y = back.point.y + PLAYER_RADIUS;
    }
}

void Player::SlopeSteepness(Mesh mesh, Model model) {
    if (!touchingGround) return;

    // Checks slope steepness and sets the slope steepness modifier.
    RayCollision slopefront = GetRayCollisionMesh(Ray{position + (direction * PLAYER_RADIUS * 0.75f), (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    RayCollision slopeback = GetRayCollisionMesh(Ray{position - (direction * PLAYER_RADIUS * 0.75f), (Vector3){ 0.0f, -1.0f, 0.0f } }, mesh, model.transform);
    if (!slopefront.hit || !slopeback.hit) return;

    slopeMovementModifier = 1.0f + ((slopefront.distance - slopeback.distance) * SLOPE_STEEPNESS_IMPACT);
    // Caps the minimum and maximum slope movement modifier.
    if (slopeMovementModifier < 0.5f) { slopeMovementModifier = 0.5f; }
    if (slopeMovementModifier > 1.5f) { slopeMovementModifier = 1.5f; }
}