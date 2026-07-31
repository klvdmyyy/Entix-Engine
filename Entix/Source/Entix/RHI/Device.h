#pragma once

#include "Entix/Core/Result.h"

namespace Entix
{
    class Window;
}

namespace Entix::RHI
{
    struct ShaderCompilationData;
    class Shader;

    class Swapchain;

    enum class GraphicsApi
    {
        Vulkan = 0, //< Default option
    };

    class Device
    {
    public:
        virtual ~Device() = default;

        virtual Result<Shader*> CreateShader(const ShaderCompilationData& compilationData) = 0;
        virtual Result<Swapchain*> CreateSwapchain(Window& window) = 0;
    };
}