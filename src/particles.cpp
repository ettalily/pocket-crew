#include "global.hpp"

Particle walkDust;

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
    DrawBillboardRec(cam.camera, texture, Rectangle{(float)frameSize * currentFrame, (float)frameSize * currentFrame, (float)frameSize, (float)frameSize}, position - (Vector3){ 0.0f, 0.2f, 0.0f}, (Vector2){ 1.0f, 1.0f }, WHITE); 
}