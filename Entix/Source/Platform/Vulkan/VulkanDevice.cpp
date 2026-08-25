#include "Platform/Vulkan/VulkanDevice.h"

#include "Platform/Vulkan/VulkanBase.h"

#include "Entix/Core/Debug/Logger.h"
#include "Entix/Core/Globals.h"

#include "Entix/Resources/ResourceManager.h"

#include "Platform/Vulkan/VulkanPipeline.h"
#include "Platform/Vulkan/VulkanShader.h"
#include "Platform/Vulkan/VulkanSwapchain.h"

#include <algorithm>

namespace Entix
{
    VulkanDevice::VulkanDevice(
        ResourceManager& resourceManager,
        Window* window
    )
        : m_resourceManager(resourceManager),
          m_window(window)
    {
        m_surface = vk::raii::SurfaceKHR(m_instance, CreateSurface(window).Unwrap());

        PickPhysicalDevice().Unwrap();
        CreateLogicalDevice().Unwrap();
    }

    VulkanDevice::~VulkanDevice()
    {
        m_resourceManager.UnloadType<VulkanShader>();
        EX_LOG(LogRHI, Debug, "Destroying VulkanDevice class");
    }

    ResourceHandle<RHI::Shader> VulkanDevice::LoadShader(const ResourceId& resourceId)
    {
        auto shader = (ResourceHandle<RHI::Shader>)m_resourceManager.Load<VulkanShader>(resourceId, m_device);
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

        EX_LOG(LogRHI, Info, "Found suitable GPU: {}", String(m_physicalDevice.getProperties().deviceName.data()));

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
