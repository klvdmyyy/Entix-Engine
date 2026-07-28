#pragma once

#include "Entix/Core/Result.h"

namespace Entix::RHI
{
    struct ShaderCompilationData;
    class Shader;

    enum class GraphicsApi
    {
        Vulkan = 0, //< Default option
    };

    class Device
    {
    public:
        virtual ~Device() = default;

        virtual Result<Shader*> CreateShader(const ShaderCompilationData& compilationData) = 0;
    };
}