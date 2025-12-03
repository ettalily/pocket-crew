#include "global.hpp"

#define CAMERA_SPEED_DIALOGUE 0.1f

Sound dialogueEnterSound, promptAcceptSound, bugCollectSound;

bool inDialogue = false;
Texture2D speechBubbleDialogue;

std::vector<std::string> dialogueText;
int dialogueLine, dialogueLineCount;
bool* dialogueOverFlag;

void StartDialogue(std::vector<std::string>* dialogue, bool* collected) {
    inDialogue = true;
    dialogueText = *dialogue;
    dialogueLine = 0;
    dialogueLineCount = dialogueText.size();
    dialogueOverFlag = collected;
    cam.speed = CAMERA_SPEED_DIALOGUE;
    if (soundOn) PlaySound(dialogueEnterSound);
}

void DialogueLogic() {
    if (!inDialogue) return;
    if (IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsKeyPressed(KEY_K) || IsKeyPressed(KEY_H)) {
        if (soundOn) PlaySound(promptAcceptSound);
        dialogueLine++;
        if (dialogueLine == dialogueLineCount - 1) PlaySound(bugCollectSound);
        if (dialogueLine != dialogueLineCount) return;

        inDialogue = false;
        pause = false;
        cam.cameraMode = Orbit;
        cam.speed = CAMERA_SPEED;
        *dialogueOverFlag = true;
        CurrentBugUpdate();
    }
}

void DrawDialogue() {
    if (!inDialogue) return;
    DrawTextureEx(speechBubbleDialogue, (Vector2){ (GetScreenWidth() * 0.5f) - ((GetScreenHeight() * 0.012f) * 0.5f * 128.0f), (GetScreenHeight() * 0.2f) - ((GetScreenHeight() * 0.005f) * 0.5f * 64.0f) }, 0.0f, GetScreenHeight() * 0.012f, WHITE);
    DrawTextScreenScaled(GetFontDefault(), dialogueText[dialogueLine].c_str(), 0.5f, 0.2f, 0.045f, 0.005f, 0.5f, DARKBLUE);
}