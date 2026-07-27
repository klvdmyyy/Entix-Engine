#pragma once

#include "Entix/RHI/Device.h"

#include "Entix/Core/Result.h"

#include <array>

#include <vulkan/vulkan_raii.hpp>

#define EX_VK_TRY(EXPR) \
    do { try { EXPR } catch(const vk::SystemError& e) { return Error(e.what()); } catch (const std::exception& e) { return Error(e.what()); } } while(0)

namespace Entix
{
    class VulkanDevice : public RHI::Device
    {
    public:
        VulkanDevice();
        ~VulkanDevice();

        static constexpr std::array<const char*, 1> VALIDATION_LAYERS = {
            "VK_LAYER_KHRONOS_validation"
        };

        static constexpr std::array<const char*, 1> REQUIRED_DEVICE_EXTENSIONS = {
            vk::KHRSwapchainExtensionName
        };

#ifdef ENTIX_BUILD_DEBUG
        static constexpr bool ENABLE_VALIDATION_LAYERS = true;
#else
        static constexpr bool ENABLE_VALIDATION_LAYERS = false;
#endif

    private:
        bool IsDeviceSuitable(const vk::raii::PhysicalDevice& physicalDevice);

        Result<void> CreateInstance();
        Result<void> SetupDebugMessenger();
        Result<void> PickPhysicalDevice();
        Result<void> CreateLogicalDevice();

        vk::raii::Context m_context;
        vk::raii::Instance m_instance = nullptr;
        vk::raii::DebugUtilsMessengerEXT m_debugMessenger = nullptr;
        vk::raii::PhysicalDevice m_physicalDevice = nullptr;
        vk::raii::Device m_device = nullptr;
        vk::raii::Queue m_graphicsQueue = nullptr;
    };
}