#include "Platform/Vulkan/VulkanSwapchain.h"

#include "Platform/Vulkan/VulkanBase.h"

#include "Entix/Core/Globals.h"
#include "Entix/Core/Debug/Logger.h"

#include <algorithm>
#include <limits>

namespace Entix
{
    VulkanSwapchain::VulkanSwapchain(
        vk::raii::PhysicalDevice& physicalDevice,
        vk::raii::Device& device,
        Window& window,
        vk::raii::SurfaceKHR& surface
    ) : m_physicalDevice(physicalDevice),
        m_device(device),
        m_window(window),
        m_surface(surface)
    {
        CreateSwapchain().Unwrap();
        CreateImageViews().Unwrap();
    }

    VulkanSwapchain::~VulkanSwapchain()
    {
        EX_LOG(LogRHI, Debug, "Destroying VulkanSwapchain class");
    }

    Result<vk::SurfaceFormatKHR> VulkanSwapchain::ChooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
    {
        if(availableFormats.empty())
            return Error("No available surface formats provided!");

        const auto formatIt = std::ranges::find_if(
            availableFormats,
            [](const auto& format) { return format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear; }
        );

        if(formatIt == availableFormats.end())
        {
            EX_LOG(LogRHI, Warning, "Can't found surface format with required capabilities. Using first available format.");
            return availableFormats[0];
        }
        else
        {
            return *formatIt;
        }
    }

    Result<vk::PresentModeKHR> VulkanSwapchain::ChoosePresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
    {
        if(availablePresentModes.empty())
            return Error("No available present modes provided!");

        if(!std::ranges::any_of(
            availablePresentModes,
            [](const vk::PresentModeKHR value)
            {
                return vk::PresentModeKHR::eFifo == value;
            }
        ))
        {
            return Error("FIFO present mode always required as backup version!");
        }

        bool hasMailbox = std::ranges::any_of(
            availablePresentModes,
            [](const vk::PresentModeKHR value)
            {
                return vk::PresentModeKHR::eMailbox == value;
            }
        );

        if(hasMailbox)
        {
            EX_LOG(LogRHI, Info, "Choosed present mode: 'Mailbox'");
            return vk::PresentModeKHR::eMailbox;
        }
        else
        {
            EX_LOG(LogRHI, Info, "Choosed present mode: 'Fifo'");
            return vk::PresentModeKHR::eFifo;
        }
    }

    Result<vk::Extent2D> VulkanSwapchain::ChooseExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
    {
        if(capabilities.currentExtent.width != std::numeric_limits<Uint32>::max())
        {
            return capabilities.currentExtent;
        }

        EX_LET_TRY(width, m_window.GetWidth());
        EX_LET_TRY(height, m_window.GetHeight());

        return vk::Extent2D {
            std::clamp<Uint32>(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
            std::clamp<Uint32>(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height),
        };
    }

    Result<Uint32> VulkanSwapchain::ChooseMinImageCount(const vk::SurfaceCapabilitiesKHR& capabilities)
    {
        auto minImageCount = std::max(3u, capabilities.minImageCount);
        if((0 < capabilities.maxImageCount) && (capabilities.maxImageCount < minImageCount))
        {
            minImageCount = capabilities.maxImageCount;
        }

        return minImageCount;
    }

    Result<void> VulkanSwapchain::CreateSwapchain()
    {
        EX_LOG(LogRHI, Info, "Creating Vulkan Swapchain");

        vk::SurfaceCapabilitiesKHR surfaceCapabilities = m_physicalDevice.getSurfaceCapabilitiesKHR(m_surface);

        EX_LET_TRY(swapchainExtent, ChooseExtent(surfaceCapabilities));
        EX_LET_TRY(minImageCount, ChooseMinImageCount(surfaceCapabilities));

        std::vector<vk::SurfaceFormatKHR> availableSurfaceFormats = m_physicalDevice.getSurfaceFormatsKHR(m_surface);

        EX_LET_TRY(swapchainSurfaceFormat, ChooseSurfaceFormat(availableSurfaceFormats));
        EX_LET_TRY(swapchainPresentMode, ChoosePresentMode(m_physicalDevice.getSurfacePresentModesKHR(m_surface)));

        vk::SwapchainCreateInfoKHR createInfo;
        createInfo.surface = *m_surface;
        createInfo.minImageCount = minImageCount;
        createInfo.imageFormat = swapchainSurfaceFormat.format;
        createInfo.imageColorSpace = swapchainSurfaceFormat.colorSpace;
        createInfo.imageExtent = swapchainExtent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        createInfo.preTransform = surfaceCapabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = swapchainPresentMode;
        createInfo.clipped = true;

        EX_VK_TRY(
            m_swapchain = vk::raii::SwapchainKHR(m_device, createInfo);
            m_swapchainImages = m_swapchain.getImages();
        );

        m_swapchainSurfaceFormat = swapchainSurfaceFormat;

        return {};
    }

    Result<void> VulkanSwapchain::CreateImageViews()
    {
        EX_LOG(LogRHI, Info, "Creating Vulkan Image Views");

        if(!m_swapchainImageViews.empty())
            return Error("Swapchain image views are already created!");
        
        vk::ImageViewCreateInfo createInfo;
        createInfo.viewType = vk::ImageViewType::e2D;
        createInfo.format = m_swapchainSurfaceFormat.format;
        createInfo.subresourceRange = { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 };
        createInfo.components = {
            vk::ComponentSwizzle::eIdentity, 
            vk::ComponentSwizzle::eIdentity,
            vk::ComponentSwizzle::eIdentity,
            vk::ComponentSwizzle::eIdentity
        };
        
        EX_VK_TRY(
            for(auto& image : m_swapchainImages)
            {
                createInfo.image = image;
                m_swapchainImageViews.emplace_back(m_device, createInfo);
            }
        );

        return {};
    }
}