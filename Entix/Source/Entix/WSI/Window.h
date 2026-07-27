#pragma once

#include "Entix/Core/Result.h"

#include "Entix/WSI/Base.h"

namespace Entix
{

    class Window
    {
    public:
        virtual ~Window() = default;

        virtual Result<void*> GetWindowHandle() const noexcept = 0;

        virtual Result<Uint32> GetWidth() const noexcept = 0;
        virtual Result<Uint32> GetHeight() const noexcept = 0;

        virtual Result<void> GrabCursor(bool value) noexcept = 0;

        virtual Result<void> ToggleGrabCursor() noexcept = 0;
    };
}