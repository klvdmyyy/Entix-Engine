#pragma once

#include "Entix/Core/Result.h"

#include "Entix/RHI/Shader.h"

#include "Entix/Resources/ResourceHandle.h"

namespace Entix
{
    class Window;
}

namespace Entix::RHI
{
    struct ShaderCompilationData;
    class Shader;

    class Swapchain;

    class GraphicsPipelineSpecification;
    class GraphicsPipeline;

    enum class GraphicsApi
    {
        Vulkan = 0, //< Default option
    };

    class Device
    {
    public:
        virtual ~Device() = default;

        virtual ResourceHandle<Shader> LoadShader(const ResourceId& resourceId) = 0;

        virtual Result<Swapchain*> CreateSwapchain(Window& window) = 0;
        // virtual Result<GraphicsPipeline*> CreateGraphicsPipeline(const GraphicsPipelineSpecification& spec) = 0;
    };
}