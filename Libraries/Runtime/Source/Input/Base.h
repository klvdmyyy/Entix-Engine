#pragma once

#include "Core/Base.h"
#include "Core/Types.h"

// ---------------------------------------------------------------------
// Тип устройства ввода (Клавиатура, геймпад и т.д)
// ---------------------------------------------------------------------
enum class InputDevice {
    Unknown = 0,

    Keyboard,
};

namespace Input {
    void GetCursorPosition(float& x, float& y);
    
    bool IsKeyPressed(Uint8 scancode);
}