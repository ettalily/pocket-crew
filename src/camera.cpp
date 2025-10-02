#include "global.hpp"

GameCamera cam;

void GameCamera::CameraInit() {
    camera.projection = CAMERA_PERSPECTIVE;
    camera.fovy = 40.0f;
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.position = (Vector3){ 10.0f, 7.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
}

void GameCamera::Update() {
    look = player.position;
    cam.camera.target = look;
    cam.camera.position = look + offset;
}

