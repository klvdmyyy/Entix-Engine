#include "Platform/Vulkan/VulkanShader.h"

#include "Platform/Vulkan/VulkanBase.h"

#include "Entix/Core/Globals.h"
#include "Entix/Core/Debug/Logger.h"

namespace Entix
{
    VulkanShader::VulkanShader(
        vk::raii::Device& device,
        const RHI::ShaderCompilationData& compilationData
    ) : m_compilationData(compilationData), m_device(device)
    {
        CreateShaderModule().Unwrap();
    }

    VulkanShader::~VulkanShader()
    {
        EX_LOG(LogRHI, Debug, "Destroying VulkanShader class");
    }

    Result<void> VulkanShader::CreateShaderModule()
    {
        EX_LOG(LogRHI, Debug, "Creating shader module. Shader code size: {}", m_compilationData.code.size());

        vk::ShaderModuleCreateInfo createInfo;
        createInfo.codeSize = m_compilationData.code.size();
        createInfo.pCode = reinterpret_cast<const Uint32*>(m_compilationData.code.data());

        EX_VK_TRY(
            m_shaderModule = vk::raii::ShaderModule(m_device, createInfo);
        );

        return {};
    }
}