// -*- mode: c++; -*-
#pragma once

#include "WSI/Window.h"

#include <SDL3/SDL.h>

class ERUNTIME_API WindowSDL : public Window {
 public:
    WindowSDL(const WindowSpecification& spec);
    ~WindowSDL();

    void Update() final;
        
    void* GetWindowHandle() const final
    {
        return static_cast<void*>(m_window);
    }

    void EnableGUIUpdate() final;

 private:
    SDL_Window* m_window;

    bool m_guiEnabled = false;
};