#include "Platform/Vulkan/VulkanDevice.h"

#include "Platform/Vulkan/VulkanBase.h"

#include "Entix/Core/Debug/Logger.h"
#include "Entix/Core/Globals.h"

#include "Entix/WSI/Base.h"

#include "Entix/Resources/ResourceManager.h"

#include "Platform/Vulkan/VulkanPipeline.h"
#include "Platform/Vulkan/VulkanShader.h"
#include "Platform/Vulkan/VulkanSwapchain.h"

#include <ranges>

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

    VulkanDevice::VulkanDevice(Window* window)
        : m_window(window)
    {
        CreateInstance().Unwrap();
        SetupDebugMessenger().Unwrap();

        m_surface = vk::raii::SurfaceKHR(m_instance, CreateSurface(window).Unwrap());

        PickPhysicalDevice().Unwrap();
        CreateLogicalDevice().Unwrap();
    }

    VulkanDevice::~VulkanDevice()
    {
        ResourceManager::Instance()->UnloadType<VulkanShader>();
        EX_LOG(LogRHI, Debug, "Destroying VulkanDevice class");
    }

    ResourceHandle<RHI::Shader> VulkanDevice::LoadShader(const ResourceId& resourceId)
    {
        auto shader = (ResourceHandle<RHI::Shader>)ResourceManager::Instance()->Load<VulkanShader>(resourceId, m_device);
        EX_LOG(LogTemp, Info, "Vulkan Shader Id: {}", shader.GetId().GetFilenameString());
        return shader;
    }

    Result<RHI::Swapchain*> VulkanDevice::CreateSwapchain(Window& window)
    {
        if((&window) == m_window && m_surface != nullptr)
        {
            auto res = new VulkanSwapchain(m_physicalDevice, m_device, window, std::move(m_surface));
            m_surface = nullptr;
            return res;
        }
        else
        {
            EX_LET_TRY(surface, CreateSurface(&window));
            return new VulkanSwapchain(m_physicalDevice, m_device, window, vk::raii::SurfaceKHR(m_instance, surface));
        }
    }

    Result<RHI::GraphicsPipeline*> VulkanDevice::CreateGraphicsPipeline(const RHI::GraphicsPipelineSpecification& spec, const std::vector<ResourceHandle<RHI::Shader>>& shaders)
    {
        return new VulkanGraphicsPipeline(m_device, spec, shaders);
    }

    Result<void> VulkanDevice::CreateInstance()
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

        EX_LET_TRY(wsiRequirements, WSI::GetRequiredVulkanInstanceExtensions());
        requiredExtensions.assign(wsiRequirements.begin(), wsiRequirements.end());

        if constexpr(ENABLE_VALIDATION_LAYERS)
        {
            requiredLayers.insert(requiredLayers.end(), VALIDATION_LAYERS.begin(), VALIDATION_LAYERS.end());
            requiredExtensions.push_back(vk::EXTDebugUtilsExtensionName);
        }

        EX_LOG(LogRHI, Debug, "Required vulkan instance extensions: {}", requiredExtensions);
        EX_LOG(LogRHI, Debug, "Vulkan instance extensions required by WSI: {}", wsiRequirements);

        auto layerProperties = m_context.enumerateInstanceLayerProperties();
        auto unsupportedLayerIt =
            std::ranges::find_if(requiredLayers, [&layerProperties](const auto& requiredLayer) {
                return std::ranges::none_of(layerProperties, [requiredLayer](const auto& layerProperty) {
                    return strcmp(layerProperty.layerName, requiredLayer) == 0;
                });
            });

        if(unsupportedLayerIt != requiredLayers.end())
        {
            return Error("Required layer not supported: " + String(*unsupportedLayerIt));
        }

        vk::InstanceCreateInfo createInfo;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledLayerCount = static_cast<Uint32>(requiredLayers.size());
        createInfo.ppEnabledLayerNames = requiredLayers.data();
        createInfo.enabledExtensionCount = static_cast<Uint32>(requiredExtensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        EX_VK_TRY(
            m_instance = vk::raii::Instance(m_context, createInfo);
        );

        return {};
    }

    Result<void> VulkanDevice::SetupDebugMessenger()
    {
        if constexpr(!ENABLE_VALIDATION_LAYERS) return {};

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

        EX_VK_TRY(
            m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(createInfo);
        );

        return {};
    }

    Result<vk::SurfaceKHR> VulkanDevice::CreateSurface(Window* window)
    {
        if(!window)
        {
            return Error(
                "No window provided. "
                "To create rendering vulkan device you need to provide at least one window for validation!"
            );
        }

        EX_LET_TRY(rawSurface, window->CreateVulkanSurface(*m_instance));

        return vk::SurfaceKHR(static_cast<VkSurfaceKHR>(rawSurface));
    }

    bool VulkanDevice::IsDeviceSuitable(const vk::raii::PhysicalDevice& physicalDevice)
    {
        bool supportsVulkan1_3 = physicalDevice.getProperties().apiVersion >= vk::ApiVersion13;

        auto queueFamilies = physicalDevice.getQueueFamilyProperties();
        bool supportsGraphics = std::ranges::any_of(
            queueFamilies,
            [](const auto& qfp) {
                return !!( qfp.queueFlags & vk::QueueFlagBits::eGraphics );
            }
        );

        auto availableDeviceExtensions = physicalDevice.enumerateDeviceExtensionProperties();
        bool supportsAllRequiredExtensions = std::ranges::all_of(
            REQUIRED_DEVICE_EXTENSIONS,
            [&availableDeviceExtensions](const auto& requiredDeviceExtension) {
                return std::ranges::any_of(
                    availableDeviceExtensions,
                    [requiredDeviceExtension](const auto& availableDeviceExtension) {
                        return strcmp( availableDeviceExtension.extensionName, requiredDeviceExtension ) == 0;
                    }
                );
            }
        );

        // Check if the physicalDevice supports the required features (shader draw parameters, dynamic rendering and extended dynamic state)
        auto features                 = physicalDevice.template getFeatures2<vk::PhysicalDeviceFeatures2,
                                                                            vk::PhysicalDeviceVulkan11Features,
                                                                            vk::PhysicalDeviceVulkan13Features,
                                                                            vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
        bool supportsRequiredFeatures = features.template get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters &&
                                        features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
                                        features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;

        // Return true if the physicalDevice meets all the criteria
        return supportsVulkan1_3 && supportsGraphics && supportsAllRequiredExtensions && supportsRequiredFeatures;
    }

    Result<void> VulkanDevice::PickPhysicalDevice()
    {
        EX_LOG(LogRHI, Info, "Finding a suitable for Vulkan GPU.");

        auto physicalDevices = m_instance.enumeratePhysicalDevices();

        if(physicalDevices.empty())
            return Error("Failed to find GPUs with Vulkan support!");

        const auto devIter = std::ranges::find_if(physicalDevices, [&](const auto& physicalDevice) {
            return IsDeviceSuitable(physicalDevice);
        });

        if(devIter == physicalDevices.end())
            return Error("Failed to find a suitable GPU!");

        m_physicalDevice = *devIter;

        EX_LOG(LogRHI, Info, "Found suitable GPU: {}", String(m_physicalDevice.getProperties().deviceName));

        return {};
    }
    
    Result<void> VulkanDevice::CreateLogicalDevice()
    {
        EX_LOG(LogRHI, Info, "Creating Vulkan logical device.");

        std::vector<vk::QueueFamilyProperties> queueFamilyProperties = m_physicalDevice.getQueueFamilyProperties();
        Uint32 graphicsIndex = 0;
        bool graphicsIndexFound = false;

        for(Uint32 qfpIndex = 0; qfpIndex < queueFamilyProperties.size(); qfpIndex++)
        {
            if((queueFamilyProperties[qfpIndex].queueFlags & vk::QueueFlagBits::eGraphics) &&
                m_physicalDevice.getSurfaceSupportKHR(qfpIndex, *m_surface))
            {
                graphicsIndex = qfpIndex;
                graphicsIndexFound = true;
                break;
            }
        }

        if(!graphicsIndexFound)
        {
            return Error("Failed to find a vulkan queue for graphics and present.");
        }

        vk::DeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.queueFamilyIndex = graphicsIndex;

        float queuePriority = 0.5f;

        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        // Create a chain of feature structures
        vk::StructureChain<vk::PhysicalDeviceFeatures2,
                           vk::PhysicalDeviceVulkan11Features,
                           vk::PhysicalDeviceVulkan13Features,
                           vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
            featureChain;

        featureChain.get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters = true;
        featureChain.get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering = true;
        featureChain.get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState = true;

        vk::DeviceCreateInfo createInfo;
        createInfo.pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>();
        createInfo.queueCreateInfoCount = 1,
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.enabledExtensionCount = static_cast<Uint32>(REQUIRED_DEVICE_EXTENSIONS.size());
        createInfo.ppEnabledExtensionNames = REQUIRED_DEVICE_EXTENSIONS.data();

        EX_VK_TRY(
            m_device = vk::raii::Device(m_physicalDevice, createInfo);
            m_graphicsQueue = vk::raii::Queue(m_device, graphicsIndex, 0);
        );

        return {};
    }
}
