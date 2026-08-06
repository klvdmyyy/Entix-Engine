#include "Platform/Vulkan/VulkanPipeline.h"

#include "Platform/Vulkan/VulkanBase.h"
#include "Platform/Vulkan/VulkanShader.h"

#include "Entix/Core/Globals.h"
#include "Entix/Core/Debug/Logger.h"
#include "Entix/Core/Events/Dispatcher.h"

#include "Entix/Resources/Resource.h"
#include "vulkan/vulkan.hpp"

namespace Entix
{
    namespace Utils
    {
        vk::ShaderStageFlagBits RHIShaderStageToVulkan(RHI::ShaderStage stage)
        {
            switch(stage)
            {
                case RHI::ShaderStage::Vertex: return vk::ShaderStageFlagBits::eVertex;
                case RHI::ShaderStage::Fragment: return vk::ShaderStageFlagBits::eFragment;
            }

            Panic("Unreachable");
        }
    }

    VulkanGraphicsPipeline::VulkanGraphicsPipeline(
        vk::raii::Device& device,
        const RHI::GraphicsPipelineSpecification& spec,
        const std::vector<ResourceHandle<RHI::Shader>>& shaders
    ) : m_spec(spec), m_shaders(shaders), m_device(device)
    {
        EventBus::AddListener(this, EventCategory::Resources);

        CreateGraphicsPipeline().Unwrap();
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

        m_pipeline.clear();

        if(auto res = CreateGraphicsPipeline(); res.IsError())
        {
            EX_LOG(LogRHI, Fatal, "Failed to recreate graphics pipeline! Error: {}", res.UnwrapErr());
        }
    }

    Result<void> VulkanGraphicsPipeline::CreateGraphicsPipeline()
    {
        EX_LOG(LogRHI, Info, "Creating graphics pipeline.");

        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;

        for(ResourceHandle<VulkanShader> shader : m_shaders)
        {
            EX_LOG(LogRHI, Trace, "Processing shader: {}", shader.GetId().GetFilenameString());

            shader->EachStage([&](RHI::ShaderStage stage){
                if(m_foundStages & stage)
                {
                    EX_LOG(LogRHI, Warning, "Repeated {} shader stage in pipeline! Shader: {}", ToString(stage), shader->GetResourceId().GetFilenameString());
                    return;
                }

                EX_LOG(LogRHI, Debug, "Attach {} shader stage to pipeline. Shader: {}", ToString(stage), shader->GetResourceId().GetFilenameString());

                vk::PipelineShaderStageCreateInfo createInfo;

                createInfo.stage = Utils::RHIShaderStageToVulkan(stage);
                createInfo.module = shader->GetShaderModule();
                createInfo.pName = RHI::ShaderCompiler::Instance()->GetStageEntryPoint(stage);

                shaderStages.push_back(createInfo);

                m_foundStages |= stage;
            });
        }

        std::vector<vk::DynamicState> dynamicStates = {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor,
        };

        vk::PipelineDynamicStateCreateInfo dynamicState;
        dynamicState.dynamicStateCount = static_cast<Uint32>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo;

        vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
        inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;

        // TODO: Does we really need it if we using dynamic states of Viewports and Scissors ?!!
        vk::Viewport viewport{0.0f, 0.0f, 800.0f, 600.0f};
        vk::Rect2D scissor{vk::Offset2D{0, 0}, vk::Extent2D{800, 600}};

        vk::PipelineViewportStateCreateInfo viewportState;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        vk::PipelineRasterizationStateCreateInfo rasterizer;
        rasterizer.depthClampEnable        = vk::False;
        rasterizer.rasterizerDiscardEnable = vk::False;
        rasterizer.polygonMode             = vk::PolygonMode::eFill;
        rasterizer.cullMode                = vk::CullModeFlagBits::eBack;
        rasterizer.frontFace               = vk::FrontFace::eClockwise;
        rasterizer.depthBiasEnable         = vk::False;
        rasterizer.lineWidth               = 1.0f;

        vk::PipelineMultisampleStateCreateInfo multisampling;
        multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
        multisampling.sampleShadingEnable = vk::False;

        vk::PipelineColorBlendAttachmentState colorBlendAttachment;
        colorBlendAttachment.blendEnable         = vk::False;
        colorBlendAttachment.colorWriteMask      = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
        colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
        colorBlendAttachment.colorBlendOp        = vk::BlendOp::eAdd;
        colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
        colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
        colorBlendAttachment.alphaBlendOp        = vk::BlendOp::eAdd;

        vk::PipelineColorBlendStateCreateInfo colorBlending;
        colorBlending.logicOpEnable = vk::False;
        colorBlending.logicOp = vk::LogicOp::eCopy;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;

        vk::PipelineLayoutCreateInfo layoutCreateInfo;
        layoutCreateInfo.setLayoutCount = 0;
        layoutCreateInfo.pushConstantRangeCount = 0;

        EX_VK_TRY(
            m_pipelineLayout = vk::raii::PipelineLayout(m_device, layoutCreateInfo);
        );

        // TODO: In vulkan tutorial it taken from Swapchain Surface Format
        auto format = vk::Format::eB8G8R8A8Srgb;

        vk::PipelineRenderingCreateInfo renderingCreateInfo;
        renderingCreateInfo.colorAttachmentCount = 1;
        renderingCreateInfo.pColorAttachmentFormats = &format;

        // TODO: We also skip render pass here.
        // I need to watch vulkan-tutorial for `vulkan.h` C API to understand
        // purposes of Render Pass things and how to organize it.
        vk::StructureChain<vk::GraphicsPipelineCreateInfo, vk::PipelineRenderingCreateInfo> pipelineCreateInfoChain{
            vk::GraphicsPipelineCreateInfo{
                {},
                static_cast<Uint32>(shaderStages.size()),
                shaderStages.data(),
                &vertexInputInfo,
                &inputAssembly,
                {},
                &viewportState,
                &rasterizer,
                &multisampling,
                {},
                &colorBlending,
                &dynamicState,
                m_pipelineLayout,
            },
            renderingCreateInfo
        };

        EX_VK_TRY(
            m_pipeline = vk::raii::Pipeline(m_device, nullptr, pipelineCreateInfoChain.get<vk::GraphicsPipelineCreateInfo>());
        );

        return {};
    }
}