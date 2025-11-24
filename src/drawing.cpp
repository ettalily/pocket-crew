#include "global.hpp"

// Draws text using screen percentages, so that it keeps a consistent size and position on different screen shapes and sizes.
void DrawTextScreenScaled(Font font, const char *text, float posPercentX, float posPercentY, float sizePercent, float spacingPercent, float posAlignmentPercent, Color color) {
    Vector2 textScale = MeasureTextEx(font, text, GetScreenHeight() * sizePercent, GetScreenHeight() * spacingPercent);
    DrawTextEx(font, text, (Vector2){ GetScreenWidth() * posPercentX, GetScreenHeight() * posPercentY } - (Vector2){ textScale.x * posAlignmentPercent, textScale.y * 0.5f }, GetScreenHeight() * sizePercent, GetScreenHeight() * spacingPercent, color);
}

void Draw() {
    // Drawing.
    BeginDrawing();
    ClearBackground(LIGHTGRAY);
    BeginMode3D(cam.camera);
    BeginShaderMode(shader);
        // Draw water.
        DrawPlane(Vector3Zero(), (Vector2){ 200.0f, 200.0f}, BLUE);
        // Draw player
        if (player.dived) { DrawSphere(player.position, player.radius - 0.1f, BLUE); }
        else { DrawSphere(player.position, player.radius, BLUE); }
        // Draw area models.
        for (auto it : loadedAreas) {
            it->Draw();
        }
    EndShaderMode();
        // Draw player drop shadow.
        DrawModel(dropShadow, (Vector3){ player.position.x, dropShadowY + 0.05f, player.position.z }, 1.0f - ((player.position.y - dropShadowY) * 0.13f), WHITE);
        // Draw player particles.
        DrawParticles();
    EndMode3D();
    if (pause) { DrawTextScreenScaled(GetFontDefault(), "PAUSED", 0.5f, 0.5f, 0.1f, 0.01f, 0.5f, BLACK); }
    DrawFPS(10, 10);
    EndDrawing();
}