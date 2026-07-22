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
    MouseMotionEvent(float x, float y, float xrel, float yrel)
        : k_xPosition(x), k_yPosition(y),
          k_xOffset(xrel), k_yOffset(yrel)
    {
    }

    const float k_xPosition;
    const float k_yPosition;
    const float k_xOffset;
    const float k_yOffset;

    DEFINE_EVENT_TYPE(MouseMotionEvent);
};

class MouseWheelEvent : public Event {
public:
    MouseWheelEvent(float scrollX, float scrollY, bool pressed)
        : k_scrollX(scrollX), k_scrollY(scrollY), k_pressed(pressed)
    {
    }

    const float k_scrollX;
    const float k_scrollY;
    const bool k_pressed;

    DEFINE_EVENT_TYPE(MouseWheelEvent);
};