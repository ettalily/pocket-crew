#include "global.hpp"

Particle walkDust, jumpDust, walljumpDust, diveDust, landDust;

void Particle::Draw() {
    if (!active) return;
    DrawBillboardRec(cam.camera, texture, Rectangle{(float)frameSize * currentFrame, (float)frameSize * currentFrame, (float)frameSize, (float)frameSize}, position - (Vector3){ 0.0f, 0.15f, 0.0f}, (Vector2){ 1.0f, 1.0f }, WHITE);
    
    if (pauseMenu) return;
    timer++;
    if (timer != frameSpeed) return;
    timer = 0;
    currentFrame++;
    if (currentFrame > frameCount) {
        currentFrame = 1;
        active = false;
    }
}

void DrawParticles() {
    walkDust.Draw();
    jumpDust.Draw();
    walljumpDust.Draw();
    diveDust.Draw();
    landDust.Draw();
}

void InitParticles() {
    walkDust.texture = LoadTexture("assets/textures/dust.png");
    walkDust.frameCount = 3;
    walkDust.frameSize = 16;
    walkDust.frameSpeed = 5;
    jumpDust.texture = LoadTexture("assets/textures/jumpdust.png");
    jumpDust.frameCount = 3;
    jumpDust.frameSize = 18;
    jumpDust.frameSpeed = 13;
    walljumpDust.texture = LoadTexture("assets/textures/walljumpdust.png");
    walljumpDust.frameCount = 3;
    walljumpDust.frameSize = 18;
    walljumpDust.frameSpeed = 5;
    diveDust.texture = LoadTexture("assets/textures/divedust.png");
    diveDust.frameCount = 3;
    diveDust.frameSize = 18;
    diveDust.frameSpeed = 7;
    landDust.texture = LoadTexture("assets/textures/landdust.png");
    landDust.frameCount = 3;
    landDust.frameSize = 18;
    landDust.frameSpeed = 8;
}

void SpawnParticle(Particle& particle) {
    particle.position = player.position;
    particle.active = true;
    particle.timer = 0;
    particle.currentFrame = 1;
}

void UnloadParticles() {
    UnloadTexture(walkDust.texture);
    UnloadTexture(jumpDust.texture);
    UnloadTexture(walljumpDust.texture);
    UnloadTexture(diveDust.texture);
    UnloadTexture(landDust.texture);
}