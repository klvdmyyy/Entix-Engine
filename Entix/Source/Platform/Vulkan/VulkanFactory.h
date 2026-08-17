#pragma once

#include "Entix/Core/Base.h"
#include "Entix/RHI/Factory.h"

#include <array>

#include <vulkan/vulkan_raii.hpp>

namespace Entix
{
    class VulkanFactory final : public RHI::Factory
    {
    public:
        ENTIX_API VulkanFactory(
            const String& applicationName,
            const Version& applicationVersion,
            const std::vector<const char*>& requiredExtensions
        );
        ENTIX_API ~VulkanFactory();

        VulkanFactory(const VulkanFactory&) = delete;
        VulkanFactory& operator=(const VulkanFactory&) = delete;
        VulkanFactory(VulkanFactory&&) = delete;
        VulkanFactory& operator=(VulkanFactory&&) = delete;

        ENTIX_API std::vector<RHI::GpuInfo> GetSupportedGpuInfos() override;
        ENTIX_API Result<RHI::Device*> CreateGpuHandle(const RHI::GpuInfo& info) override;

        static constexpr std::array<const char*, 1> VALIDATION_LAYERS = {
            "VK_LAYER_KHRONOS_validation"
        };

    private:
#ifdef ENTIX_BUILD_DEBUG
        static constexpr bool ENABLE_VALIDATION_LAYERS = true;
#else
        static constexpr bool ENABLE_VALIDATION_LAYERS = false;
#endif

        ENTIX_API void CreateInstance();
        ENTIX_API void SetupDebugMessenger();

        ENTIX_API static RHI::GpuInfo GetPhysicalDeviceInfo(vk::raii::PhysicalDevice& physicalDevice, Usize index);

        const String k_applicationName;
        const Version k_applicationVersion;
        const std::vector<const char*> k_requiredExtensions;

        vk::raii::Context m_context{};
        vk::raii::Instance m_instance = nullptr;
        vk::raii::DebugUtilsMessengerEXT m_debugMessenger = nullptr;
    };
}
