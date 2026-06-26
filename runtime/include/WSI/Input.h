#pragma once

#include "Core/Base.h"
#include "Core/Types.h"
#include "Core/String.h"

#include "Core/EventSystem.h"

namespace ERUNTIME_NAMESPACE {
    // ---------------------------------------------------------------------
    // Тип устройства ввода (Клавиатура, геймпад и т.д)
    // ---------------------------------------------------------------------
    enum class InputDevice {
        Unknown = 0,

        Keyboard,
    };

    // ---------------------------------------------------------------------
    // Событие ввода с клавиатуры
    // ---------------------------------------------------------------------
    class KeyboardEvent : public Event {
    public:
        KeyboardEvent(Uint32 code, bool pressed = true)
            : k_code(code), k_pressed(pressed)
        {
        }

        const Uint32 k_code;
        const bool k_pressed;

        DEFINE_EVENT_TYPE(KeyboardEvent);
    };

    class Input {
    public:
        static bool IsKeyPressed(Uint8 scancode);
    };
}