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
            const RHI::GraphicsPipelineSpecification& spec,
            const std::vector<ResourceHandle<RHI::Shader>>& shaders
        );
        ~VulkanGraphicsPipeline();

        void OnEvent(const Event& event) final;

        void OnShaderReloaded(const ResourceId& shaderId);

    private:
        Result<void> CreateGraphicsPipeline();

        RHI::GraphicsPipelineSpecification m_spec;
        std::vector<ResourceHandle<RHI::Shader>> m_shaders;

        vk::raii::Device& m_device;

        vk::raii::PipelineLayout m_pipelineLayout = nullptr;
        vk::raii::Pipeline m_pipeline = nullptr;

        Uint8 m_foundStages = 0;
    };
}