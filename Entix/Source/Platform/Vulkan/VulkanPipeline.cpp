#include "Platform/Vulkan/VulkanPipeline.h"

#include "Platform/Vulkan/VulkanBase.h"

#include "Entix/Core/Globals.h"
#include "Entix/Core/Debug/Logger.h"
#include "Entix/Core/Events/Dispatcher.h"

#include "Entix/Resources/Resource.h"

namespace Entix
{
    VulkanGraphicsPipeline::VulkanGraphicsPipeline(
        vk::raii::Device& device,
        const RHI::GraphicsPipelineSpecification& spec
    ) : m_spec(spec), m_device(device)
    {
        EventBus::AddListener(this, EventCategory::Resources);
    }

    VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
    {
        EventBus::RemoveListener(this);
    }

    void VulkanGraphicsPipeline::OnEvent(const Event& event)
    {
        EventBus::Dispatch<ResourceReloadedEvent>(event, [&](const ResourceReloadedEvent& event) {
            OnShaderReloaded(event.GetResourceId());
        });
    }

    void VulkanGraphicsPipeline::OnShaderReloaded(const ResourceId& shaderId)
    {
        EX_LOG(LogRHI, Debug, "Shader '{}' is reloaded. Recreating Graphics Pipeline.", shaderId.GetFilenameString());
    }
}