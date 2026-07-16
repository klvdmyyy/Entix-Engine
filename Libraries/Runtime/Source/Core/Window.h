#pragma once

#include "Core/String.h"
#include "Core/EventSystem.h"

#include <cstdint>

using WindowID = std::uint32_t;

class WindowCloseEvent : public Event
{
public:
    WindowCloseEvent(WindowID id)
        : id(id)
    {
    }

    WindowID id;

    DEFINE_EVENT_TYPE(WindowCloseEvent);
};

struct WindowSpecification
{
    std::uint32_t width;
    std::uint32_t height;
    String title;
    bool resizable = false;
    bool fullscreen = false;
};

class Window {
public:
    virtual ~Window() = default;

    virtual void Update() = 0;
    
    virtual void* GetWindowHandle() const noexcept = 0;

    virtual void GrabCursor(bool value = true) = 0;

    virtual void EnableGUIUpdate() = 0;

    static Window* Create(const WindowSpecification& spec);
};