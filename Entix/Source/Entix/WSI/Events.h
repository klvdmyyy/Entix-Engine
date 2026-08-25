#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Types.h"
#include "Entix/Core/Events/Base.h"

#include "Entix/Core/Result.h"

#include <vector>

namespace Entix
{
    EX_DEFINE_ID_TYPE(WindowId);

    class WindowEventBase
    {
    public:
        WindowEventBase(WindowId id) : m_windowId(id) {}

        inline WindowId GetWindowId() const noexcept { return m_windowId; }

    private:
        WindowId m_windowId;
    };

    class WindowCloseEvent : public Event, public WindowEventBase
    {
        EX_DEFINE_EVENT_TYPE_CATEGORY(WindowCloseEvent, EventCategory::Window);

    public:
        WindowCloseEvent(WindowId id) : WindowEventBase(id) {}
    };

    class WindowResizeEvent : public Event, public WindowEventBase
    {
        EX_DEFINE_EVENT_TYPE_CATEGORY(WindowResizeEvent, EventCategory::Window);

    public:
        WindowResizeEvent(WindowId id, Uint32 width, Uint32 height)
            : WindowEventBase(id), m_width(width), m_height(height)
        {
        }

        inline Uint32 GetWidth() const noexcept { return m_width; }
        inline Uint32 GetHeight() const noexcept { return m_height; }

    private:
        Uint32 m_width;
        Uint32 m_height;
    };

    class MouseMotionEvent : public Event, public WindowEventBase
    {
        EX_DEFINE_EVENT_TYPE_CATEGORY(MouseMotionEvent,
            EventCategory::Window | EventCategory::Input | EventCategory::Mouse);

    public:
        MouseMotionEvent(WindowId id, float x, float y, float xRel, float yRel)
            : WindowEventBase(id),
              m_xPosition(x), m_yPosition(y),
              m_xRelative(xRel), m_yRelative(yRel)
        {
        }

        inline float GetPositionX() const noexcept { return m_xPosition; }
        inline float GetPositionY() const noexcept { return m_yPosition; }

        inline float GetRelativeX() const noexcept { return m_xRelative; }
        inline float GetRelativeY() const noexcept { return m_yRelative; }

    private:
        float m_xPosition, m_yPosition;
        float m_xRelative, m_yRelative;
    };
}