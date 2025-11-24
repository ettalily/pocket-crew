#include "global.hpp"

Particle walkDust;
Particle jumpDust;
Particle walljumpDust;
Particle diveDust;

void Particle::DrawParticles() {
    if (!active)
    {
        return;
    }
    timer ++;
    if (timer == frameSpeed) {
        timer = 0;
        currentFrame ++;
        if (currentFrame > frameCount) {
            currentFrame = 1;
            active = false;
            return;
        }
    }
    DrawBillboardRec(cam.camera, texture, Rectangle{(float)frameSize * currentFrame, (float)frameSize * currentFrame, (float)frameSize, (float)frameSize}, position - (Vector3){ 0.0f, 0.15f, 0.0f}, (Vector2){ 1.0f, 1.0f }, WHITE); 
}

void InitParticles() {
    walkDust.texture = LoadTexture("assets/textures/dust.png");
    walkDust.frameCount = 3;
    walkDust.frameSize = 16;
    walkDust.frameSpeed = 5;
    jumpDust.texture = LoadTexture("assets/textures/jumpdust.png");
    jumpDust.frameCount = 3;
    jumpDust.frameSize = 16;
    jumpDust.frameSpeed = 13;
    walljumpDust.texture = LoadTexture("assets/textures/walljumpdust.png");
    walljumpDust.frameCount = 3;
    walljumpDust.frameSize = 16;
    walljumpDust.frameSpeed = 4;
    diveDust.texture = LoadTexture("assets/textures/divedust.png");
    diveDust.frameCount = 3;
    diveDust.frameSize = 16;
    diveDust.frameSpeed = 5;
}

void SpawnParticle(Particle& particle) {
    particle.position = player.position;
    particle.active = true;
    particle.timer = 0;
    particle.currentFrame = 1;
}