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
        
    protected:
        Result<void> LoadInternal() final;
        Result<void> ReloadInternal() final;
        void UnloadInternal() final;

    private:
        Result<void> CreateShaderModule(const RHI::ShaderCompilationData& compilationData);

        vk::raii::Device& m_device;
        
        vk::raii::ShaderModule m_shaderModule = nullptr;
    };
}