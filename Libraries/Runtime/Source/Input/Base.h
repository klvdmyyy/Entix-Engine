#pragma once

#include "Core/Base.h"
#include "Core/Types.h"

// ---------------------------------------------------------------------
// Тип устройства ввода (Клавиатура, геймпад и т.д)
// ---------------------------------------------------------------------
enum class InputDevice {
    Unknown = 0,

    Keyboard,
    Mouse,
};

enum class ModifierKey {
    Unknown = 0,

    Ctrl,
    Alt,
    Super,
    Shift,
};

namespace Input {
    InputDevice ParseInputDevice(const String& name);
    ModifierKey ParseModifierKey(const String& name);

    void GetCursorPosition(float& x, float& y);
    bool IsButtonPressed(Uint8 button);
    
    bool IsKeyModifierPressed(ModifierKey key);
    bool IsKeyPressed(Uint8 scancode);
}