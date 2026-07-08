// -*- mode: c++; -*-
#pragma once

#include "WSI/Window.h"

#include <SDL3/SDL.h>

class ERUNTIME_API WindowSDL : public Window {
 public:
    WindowSDL(const WindowSpecification& spec);
    ~WindowSDL();

    void Update() final;
        
    void* GetWindowHandle() const noexcept final
    {
        return static_cast<void*>(m_window);
    }

    void GrabCursor(bool value = true) final;

    void EnableGUIUpdate() final;

 private:
    SDL_Window* m_window;

    bool m_guiEnabled = false;
    bool m_isCursorGrabbed = false;
};