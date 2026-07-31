#pragma once

#include "Entix/Core/Types.h"
#include "Entix/Core/Result.h"

namespace Entix
{
    struct WindowConfig
    {
        WindowConfig() = default;

        Uint32 width = 800;
        Uint32 height = 600;
        String title = "No Title";
    };

    class Window
    {
    public:
        virtual ~Window() = default;

        virtual Result<void*> GetWindowHandle() const noexcept = 0;

        virtual Result<void*> CreateVulkanSurface(void* instance) noexcept = 0;

        virtual Result<Uint32> GetWidth() const noexcept = 0;
        virtual Result<Uint32> GetHeight() const noexcept = 0;

        virtual Result<void> GrabCursor(bool value) noexcept = 0;

        virtual Result<void> ToggleGrabCursor() noexcept = 0;
    };
}