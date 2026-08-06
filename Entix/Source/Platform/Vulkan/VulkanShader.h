#pragma once

#include "Entix/RHI/Shader.h"

#include <vulkan/vulkan_raii.hpp>

namespace Entix
{
    class VulkanShader : public RHI::Shader
    {
    public:
        VulkanShader(
            const ResourceId& resourceId,
            vk::raii::Device& device
        );
        ~VulkanShader();

        bool HasStage(RHI::ShaderStage stage) const noexcept final { return m_stages & stage; }

        vk::ShaderModule GetShaderModule() const noexcept { return m_shaderModule; }
        
    protected:
        Result<void> LoadInternal() final;
        Result<void> ReloadInternal() final;
        void UnloadInternal() final;

    private:
        Result<void> CreateShaderModule(const RHI::ShaderCompilationData& compilationData);

        vk::raii::Device& m_device;
        Uint8 m_stages = 0;
        
        vk::raii::ShaderModule m_shaderModule = nullptr;
    };
}