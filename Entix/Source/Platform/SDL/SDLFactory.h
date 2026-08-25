#pragma once

#include "Entix/Core/Result.h"

#include "Entix/WSI/Window.h"

#include <SDL3/SDL.h>

#include <vector>

namespace Entix
{
    class SDLFactory
    {
    public:
        SDLFactory();
        ~SDLFactory();

        // Windowing

        Result<Window*> CreateWindow(const WindowDesc& config);

        // Events

        void PollEvents();

        // Vulkan things

        Result<std::vector<const char*>> GetRequiredVulkanInstanceExtensions();
    };
}