#pragma once

#include "Entix/RHI/Pipeline.h"

#include "Entix/Core/Events/Listener.h"
#include "Entix/Resources/ResourceId.h"

#include <vulkan/vulkan_raii.hpp>

namespace Entix
{
    class VulkanGraphicsPipeline : public RHI::GraphicsPipeline, EventListener
    {
    public:
        VulkanGraphicsPipeline(
            vk::raii::Device& device,
            const RHI::GraphicsPipelineSpecification& spec
        );
        ~VulkanGraphicsPipeline();

        void OnEvent(const Event& event) final;

        void OnShaderReloaded(const ResourceId& shaderId);

    private:
        RHI::GraphicsPipelineSpecification m_spec;

        vk::raii::Device& m_device;

        Uint8 m_enabledStages = 0;
    };
}