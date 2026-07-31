#pragma once

#include "Entix/RHI/Swapchain.h"

#include "Entix/WSI/Window.h"

#include <vulkan/vulkan_raii.hpp>

namespace Entix
{
    class VulkanSwapchain : public RHI::Swapchain
    {
    public:
        VulkanSwapchain(
            vk::raii::Device& device,
            Window& window,
            vk::raii::SurfaceKHR surface
        );
        ~VulkanSwapchain();

    private:
        Result<void> CreateSurface();

        vk::raii::Device& m_device;
        Window& m_window;

        vk::raii::SurfaceKHR m_surface = nullptr;
    };
}