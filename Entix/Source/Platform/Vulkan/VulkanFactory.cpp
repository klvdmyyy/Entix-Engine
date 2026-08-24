#include "Platform/Vulkan/VulkanFactory.h"

#include "Entix/Core/Globals.h"
#include "Entix/Core/Debug/Logger.h"

#include <algorithm>

namespace Entix
{
    static VKAPI_ATTR vk::Bool32 VKAPI_CALL DebugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT       severity,
                                                          [[maybe_unused]] vk::DebugUtilsMessageTypeFlagsEXT type,
                                                          const vk::DebugUtilsMessengerCallbackDataEXT * pCallbackData,
                                                          [[maybe_unused]] void *                            pUserData)
    {
        LogLevel level = LogLevel::Count;

        if(severity >= vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
        {
            level = LogLevel::Error;
        }
        else if(severity >= vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning)
        {
            level = LogLevel::Warning;
        }
        else if(severity >= vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo)
        {
            level = LogLevel::Info;
        }
        else if(severity >= vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose)
        {
            level = LogLevel::Debug;
        }
        else
        {
            level = LogLevel::Trace;
        }

        if(type & vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral)
        {
            Logger::Instance().LogMessage(
                EX_GLOBAL_LOG_CATEGORY_NAME(VulkanGeneral),
                                          level, std::format("{}", pCallbackData->pMessage)
            );
        }
        else if(type & vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
        {
            Logger::Instance().LogMessage(
                EX_GLOBAL_LOG_CATEGORY_NAME(VulkanPerformance),
                                          level, std::format("{}", pCallbackData->pMessage)
            );
        }
        else if(type & vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation)
        {
            Logger::Instance().LogMessage(
                EX_GLOBAL_LOG_CATEGORY_NAME(VulkanValidation),
                                          level, std::format("{}", pCallbackData->pMessage)
            );
        }
        else
        {
            return vk::True;
        }

        return vk::False;
    }

    VulkanFactory::VulkanFactory(
        const String& applicationName,
        const Version& applicationVersion,
        const std::vector<const char*>& requiredExtensions
    )
        : k_applicationName(applicationName),
          k_applicationVersion(applicationVersion),
          k_requiredExtensions(requiredExtensions)
    {
        CreateInstance();
        SetupDebugMessenger();
    }

    VulkanFactory::~VulkanFactory()
    {
    }

    void VulkanFactory::CreateInstance()
    {
        EX_LOG(LogRHI, Info, "Creating vulkan instance.");

        vk::ApplicationInfo appInfo;
        appInfo.pApplicationName = "Sandbox Game";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Entix Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = vk::ApiVersion14;

        std::vector<const char*> requiredLayers;
        std::vector<const char*> requiredExtensions;

        requiredExtensions.assign(k_requiredExtensions.begin(), k_requiredExtensions.end());

        if constexpr(ENABLE_VALIDATION_LAYERS)
        {
            requiredLayers.insert(requiredLayers.end(), VALIDATION_LAYERS.begin(), VALIDATION_LAYERS.end());
            requiredExtensions.push_back(vk::EXTDebugUtilsExtensionName);
        }

        EX_LOG(LogRHI, Debug, "Required vulkan instance extensions: {}", requiredExtensions);

        auto layerProperties = m_context.enumerateInstanceLayerProperties();
        auto unsupportedLayerIt =
        std::ranges::find_if(requiredLayers, [&layerProperties](const auto& requiredLayer) {
            return std::ranges::none_of(layerProperties, [requiredLayer](const auto& layerProperty) {
                return strcmp(layerProperty.layerName, requiredLayer) == 0;
            });
        });

        if(unsupportedLayerIt != requiredLayers.end())
        {
            Panic("Required layer not supported: " + String(*unsupportedLayerIt));
        }

        vk::InstanceCreateInfo createInfo;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledLayerCount = static_cast<Uint32>(requiredLayers.size());
        createInfo.ppEnabledLayerNames = requiredLayers.data();
        createInfo.enabledExtensionCount = static_cast<Uint32>(requiredExtensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        try
        {
            m_instance = vk::raii::Instance(m_context, createInfo);
        }
        catch(vk::SystemError& e)
        {
            Panic(e.what());
        }
    }

    void VulkanFactory::SetupDebugMessenger()
    {
        if constexpr(!ENABLE_VALIDATION_LAYERS) return;

        EX_LOG(LogRHI, Debug, "Setting up Vulkan debug messenger.");

        vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
        vk::DebugUtilsMessageTypeFlagsEXT     messageTypeFlags(
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

        vk::DebugUtilsMessengerCreateInfoEXT createInfo;
        createInfo.messageSeverity = severityFlags;
        createInfo.messageType = messageTypeFlags;
        createInfo.pfnUserCallback = &DebugCallback;

        try
        {
            m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(createInfo);
        }
        catch(vk::SystemError& e)
        {
            Panic(e.what());
        }
    }

    std::vector<RHI::GpuInfo> VulkanFactory::GetSupportedGpuInfos()
    {
        auto physicalDevices = m_instance.enumeratePhysicalDevices();
        std::vector<RHI::GpuInfo> gpuInfos;

        for(Usize i = 0; i < physicalDevices.size(); i++)
        {
            gpuInfos.emplace_back(GetPhysicalDeviceInfo(physicalDevices[i], i));
        }

        return gpuInfos;
    }

    Result<RHI::Device*> VulkanFactory::CreateGpuHandle(const RHI::GpuInfo& info)
    {
        auto physicalDevices = m_instance.enumeratePhysicalDevices();
        if(info.index_ < physicalDevices.size())
            Panic("TODO!");
        else
            return Error("Required GPU not found. Gpu Name: " + String(info.name));
    }

    RHI::GpuInfo VulkanFactory::GetPhysicalDeviceInfo(vk::raii::PhysicalDevice& physicalDevice, Usize index)
    {
        auto properties = physicalDevice.getProperties();
        return RHI::GpuInfo{
            .name = String(properties.deviceName.data()),
            .index_ = index,
        };
    }
}
