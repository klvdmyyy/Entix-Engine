#include "Platform/Vulkan/VulkanPipeline.h"

#include "Platform/Vulkan/VulkanBase.h"

namespace Entix
{
    VulkanGraphicsPipeline::VulkanGraphicsPipeline(
        vk::raii::Device& device,
        const RHI::GraphicsPipelineSpecification& spec
    ) : m_spec(spec), m_device(device)
    {
    }

    VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
    {
    }
}