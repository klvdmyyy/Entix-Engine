#pragma once

#include "Entix/RHI/Swapchain.h"

#include "Entix/WSI/Window.h"
#include "vulkan/vulkan.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Entix
{
    class VulkanSwapchain : public RHI::Swapchain
    {
    public:
        VulkanSwapchain(
            vk::raii::PhysicalDevice& physicalDevice,
            vk::raii::Device& device,
            Window& window,
            vk::raii::SurfaceKHR& surface
        );
        ~VulkanSwapchain();

    private:
        Result<vk::SurfaceFormatKHR> ChooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
        Result<vk::PresentModeKHR> ChoosePresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
        Result<vk::Extent2D> ChooseExtent(const vk::SurfaceCapabilitiesKHR& capabilities);
        Result<Uint32> ChooseMinImageCount(const vk::SurfaceCapabilitiesKHR& capabilities);

        Result<void> CreateSwapchain();
        Result<void> CreateImageViews();

        vk::raii::PhysicalDevice& m_physicalDevice;
        vk::raii::Device& m_device;
        Window& m_window;

        vk::raii::SurfaceKHR& m_surface;
        vk::raii::SwapchainKHR m_swapchain = nullptr;
        std::vector<vk::Image> m_swapchainImages;
        vk::SurfaceFormatKHR m_swapchainSurfaceFormat;

        std::vector<vk::raii::ImageView> m_swapchainImageViews;
    };
}