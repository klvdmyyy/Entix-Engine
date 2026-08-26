#pragma once

#include "Entix/Core/Result.h"

#include "Entix/WSI/Window.h"

#include "Entix/RHI/Factory.h"

#include <SDL3/SDL.h>

#include <vector>

namespace Entix
{
    class SDLFactory
    {
    public:
        SDLFactory();
        ~SDLFactory();

        Result<Window*> CreateWindow(const WindowDesc& desc);
        Result<RHI::Factory*> CreateRHIFactory(const RHI::FactoryDesc& desc);

        void PollEvents();

    private:
        Result<std::vector<const char*>> GetRequiredVulkanInstanceExtensions();
    };
}