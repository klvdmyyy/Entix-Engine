#pragma once

#include "Entix/RHI/Shader.h"

#include "Entix/Resources/ResourceHandle.h"

namespace Entix::RHI
{
    struct GraphicsPipelineSpecification
    {
        // std::vector<ResourceHandle<Shader>> shaders;
    };

    class GraphicsPipeline
    {
    public:
        virtual ~GraphicsPipeline() = default;
    };
}