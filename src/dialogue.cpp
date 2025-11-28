#include "global.hpp"

bool inDialogue = false;

std::vector<std::string> dialogueText;
int dialogueLine, dialogueLineCount;
bool* dialogueOverFlag;

void StartDialogue(std::vector<std::string>* dialogue, bool* collected) {
    inDialogue = true;
    dialogueText = *dialogue;
    dialogueLine = 0;
    dialogueLineCount = dialogueText.size();
    dialogueOverFlag = collected;
    cam.speed = 0.1f;
}

void DialogueLogic() {
    if (!inDialogue) return;
    if (IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsGamepadButtonPressed(gamepadID, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsKeyPressed(KEY_K) || IsKeyPressed(KEY_H)) {
        dialogueLine++;
        if (dialogueLine == dialogueLineCount) {
            inDialogue = false;
            pause = false;
            cam.cameraMode = Orbit;
            cam.speed = 0.2f;
            *dialogueOverFlag = true;
        }
    }
}

void DrawDialogue() {
    if (!inDialogue) return;
    DrawTextScreenScaled(GetFontDefault(), dialogueText[dialogueLine].c_str(), 0.5f, 0.3f, 0.05f, 0.01f, 0.5f, DARKGREEN);
}