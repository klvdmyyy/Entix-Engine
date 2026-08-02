#pragma once

#include "Entix/WSI/Window.h"

#include <SDL3/SDL.h>

namespace Entix
{
    class WindowSDL : public Window
    {
    public:
        WindowSDL(const WindowConfig& config);
        ~WindowSDL();

        Result<void*> GetWindowHandle() const noexcept final;

        Result<void*> CreateVulkanSurface(void* instance) noexcept final;

        Result<Uint32> GetWidth() const noexcept final;
        Result<Uint32> GetHeight() const noexcept final;

        Result<void> GrabCursor(bool value) noexcept final;

        Result<void> ToggleGrabCursor() noexcept final;

    private:
        Result<void> Initialize();
        Result<void> Shutdown();

        const WindowConfig k_config;

        SDL_Window* m_window = nullptr;
    };
}