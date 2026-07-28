#pragma once

#include "Entix/RHI/Shader.h"

namespace Entix::RHI
{
    struct GraphicsPipelineSpecification
    {
    };

    class GraphicsPipeline
    {
    public:
        virtual ~GraphicsPipeline() = default;
    };
}