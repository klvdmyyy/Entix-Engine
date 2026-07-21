#pragma once

#include "Core/Types.h"
#include "Core/String.h"
#include "Core/EventSystem.h"

#include <cstdint>

using WindowId = std::uint32_t;

class WindowCloseEvent : public Event
{
public:
    WindowCloseEvent(WindowId id)
        : k_id(id)
    {
    }

    const WindowId k_id;

    DEFINE_EVENT_TYPE(WindowCloseEvent);
};

class WindowResizeEvent : public Event {
public:
    WindowResizeEvent(WindowId id, Uint32 w, Uint32 h)
        : k_id(id), k_width(w), k_height(h)
    {
    }

    const WindowId k_id;
    const Uint32 k_width;
    const Uint32 k_height;

    DEFINE_EVENT_TYPE(WindowResizeEvent);
};

struct WindowSpecification
{
    std::uint32_t width;
    std::uint32_t height;
    String title;
    bool resizable = false;
    bool fullscreen = false;
    bool maximized = false;
};

class Window {
public:
    virtual ~Window() = default;

    virtual void Update() = 0;

    virtual Uint32 GetWidth() const = 0;
    virtual Uint32 GetHeight() const = 0;
    
    virtual void* GetWindowHandle() const noexcept = 0;

    virtual void GrabCursor(bool value = true) = 0;

    virtual void EnableGUIUpdate() = 0;

    static Window* Create(const WindowSpecification& spec);
};