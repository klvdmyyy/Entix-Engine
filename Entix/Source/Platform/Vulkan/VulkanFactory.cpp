#include "Platform/Vulkan/VulkanFactory.h"

namespace Entix
{
    VulkanFactory::VulkanFactory(const String& applicationName, Version applicationVersion)
        : k_applicationName(applicationName),
          k_applicationVersion(applicationVersion)
    {
    }

    VulkanFactory::~VulkanFactory()
    {
    }
}
