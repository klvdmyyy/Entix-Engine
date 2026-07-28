#pragma once

#include "Entix/RHI/Pipeline.h"

#include <vulkan/vulkan_raii.hpp>

namespace Entix
{
    class VulkanGraphicsPipeline : public RHI::GraphicsPipeline
    {
    public:
        VulkanGraphicsPipeline(
            vk::raii::Device& device,
            const RHI::GraphicsPipelineSpecification& spec
        );
        ~VulkanGraphicsPipeline();

    private:
        RHI::GraphicsPipelineSpecification m_spec;

        vk::raii::Device& m_device;
    };
}