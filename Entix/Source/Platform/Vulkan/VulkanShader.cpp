#include "Platform/Vulkan/VulkanShader.h"

#include "Entix/RHI/Shader.h"
#include "Platform/Vulkan/VulkanBase.h"

#include "Entix/Core/Globals.h"
#include "Entix/Core/Debug/Logger.h"

namespace Entix
{
    VulkanShader::VulkanShader(
        const ResourceId& resourceId,
        vk::raii::Device& device
    ) : RHI::Shader(resourceId), m_device(device)
    {
    }

    VulkanShader::~VulkanShader()
    {
        EX_LOG(LogRHI, Debug, "Destroying VulkanShader class");
    }

    Result<void> VulkanShader::LoadInternal()
    {
        auto shaderc = RHI::ShaderCompiler::Instance();

        EX_LET_TRY(compilationData, shaderc->Compile(GetResourceId().GetFilepath()));

        EX_TRY(CreateShaderModule(compilationData));

        return {};
    }

    Result<void> VulkanShader::ReloadInternal()
    {
        Unload();
        EX_TRY(RHI::ShaderCompiler::Instance()->RecreateSession());
        return Load();
    }

    void VulkanShader::UnloadInternal()
    {
        m_shaderModule.clear();
    }

    Result<void> VulkanShader::CreateShaderModule(const RHI::ShaderCompilationData& compilationData)
    {
        EX_LOG(LogRHI, Debug, "Creating shader module. Shader code size: {}", compilationData.code.size());

        vk::ShaderModuleCreateInfo createInfo;
        createInfo.codeSize = compilationData.code.size();
        createInfo.pCode = reinterpret_cast<const Uint32*>(compilationData.code.data());

        EX_VK_TRY(
            m_shaderModule = vk::raii::ShaderModule(m_device, createInfo);
        );

        return {};
    }
}