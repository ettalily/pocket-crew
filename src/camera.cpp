#include "global.hpp"

CameraSettings cameraMode = Orbit;
GameCamera cam;

// Used to figure out the vector for forward relative to the camera. Returns a normal that only considers horizontal directions.
Vector3 GetForwardNormal() {
    return Vector3Normalize((Vector3){ cam.camera.target.x, 0.0f, cam.camera.target.z } - (Vector3){ cam.camera.position.x, 0.0f, cam.camera.position.z });
}

void GameCamera::CameraInit() {
    camera.projection = CAMERA_PERSPECTIVE;
    camera.fovy = 40.0f;
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.position = (Vector3){ 10.0f, 7.0f, 10.0f };
}

void GameCamera::Update() {
    switch (cameraMode) {
        // Orbits around a center point but looks at the player.
        case Orbit:
            look = player.position;
            orbits = (Vector3){ -33.0f, player.position.y, 33.0f };
            cam.camera.position = orbits + ((Vector3){ player.position.x - orbits.x, 0.0f, player.position.z - orbits.z } + (Vector3Normalize((Vector3){ player.position.x - orbits.x, 0.0f, player.position.z - orbits.z }) * offset)) + (Vector3){ 0.0f, 6.0f, 0.0f };
            cam.camera.target = look;
            break;
    }   
}