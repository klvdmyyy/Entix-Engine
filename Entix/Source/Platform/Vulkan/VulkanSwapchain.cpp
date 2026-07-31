#include "Platform/Vulkan/VulkanSwapchain.h"

namespace Entix
{
    VulkanSwapchain::VulkanSwapchain(
        vk::raii::Device& device,
        Window& window,
        vk::raii::SurfaceKHR surface
    ) : m_device(device),
        m_window(window),
        m_surface(std::move(surface))
    {
    }

    VulkanSwapchain::~VulkanSwapchain()
    {
    }
}