#pragma once

#include "Entix/Core/Globals.h"
#include "Entix/Core/Debug/Logger.h"
#include "Entix/Core/Result.h"

#include "Entix/Core/String.h"
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

namespace Entix
{
    template<>
    RHI::GraphicsApi DefaultOf<RHI::GraphicsApi>();

    template<>
    String ToString<RHI::GraphicsApi>(const RHI::GraphicsApi& api);

    template<>
    RHI::GraphicsApi FromString<RHI::GraphicsApi>(const String& str_);
}