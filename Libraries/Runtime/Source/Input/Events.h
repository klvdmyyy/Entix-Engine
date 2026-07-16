#pragma once

#include "Core/EventSystem.h"

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

class MouseMotionEvent : public Event {
public:
    MouseMotionEvent(float x, float y)
        : k_xPosition(x), k_yPosition(y)
    {
    }

    const float k_xPosition;
    const float k_yPosition;

    DEFINE_EVENT_TYPE(MouseMotionEvent);
};