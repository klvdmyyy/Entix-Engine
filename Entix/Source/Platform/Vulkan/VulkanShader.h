#pragma once

#include "Entix/Core/Memory.h"
#include "Entix/RHI/Shader.h"

#include <vulkan/vulkan_raii.hpp>

namespace Entix
{
    class VulkanShader : public RHI::Shader
    {
    public:
        VulkanShader(
            vk::raii::Device& device,
            const RHI::ShaderCompilationData& compilationData
        );
        ~VulkanShader();

        bool HasStage(RHI::ShaderStage stage) const noexcept final
        {
            return m_compilationData.stages & stage;
        }

    private:
        Result<void> CreateShaderModule();

        RHI::ShaderCompilationData m_compilationData;
        vk::raii::Device& m_device;
        vk::raii::ShaderModule m_shaderModule = nullptr;
    };
}