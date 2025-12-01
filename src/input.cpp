#include "global.hpp"

#define STICK_DEADZONE 0.05
int gamepadID = 0;
Vector2 dirInput;

void UpdateMovementAxis() {
    dirInput = Vector2Zero();
    // Checks if either left stick axis has passed the deadzone. If either has, it'll use the left stick input.
    if (abs(GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X)) >= STICK_DEADZONE || abs(GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y)) >= STICK_DEADZONE) {
        // Gets left stick input value.
        dirInput = (Vector2){ GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X), -GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y) } ;
        
        // Applies deadzones.
        if (abs(GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X)) < STICK_DEADZONE) {
            dirInput.x = 0.0f;
        }
        if (abs(GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y)) < STICK_DEADZONE) {
            dirInput.y = 0.0f;
        }
    // WASD/arrow key/d-pad input. Only runs if neither gamepad stick axis deadzone is passed, which includes when no controller is connected.
    } else {
        // Gets the normalised keyboard/arrow key/d-pad input direction vector.
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP) || IsGamepadButtonDown(gamepadID, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
            dirInput.y += 1.0f;
        }
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN) || IsGamepadButtonDown(gamepadID, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
            dirInput.y -= 1.0f;
        }
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT) || IsGamepadButtonDown(gamepadID, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
            dirInput.x += 1.0f;
        }
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT) || IsGamepadButtonDown(gamepadID, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
            dirInput.x -= 1.0f;
        }

        dirInput = Vector2Normalize(dirInput);
    }
}