#include "Entix/RHI/Factory.h"

#include "Platform/Vulkan/VulkanFactory.h"

namespace Entix::RHI
{
    Result<Scope<Factory>> Factory::Create(const FactoryDesc& desc)
    {
        switch(desc.backendApi)
        {
            case BackendApi::Vulkan:
                return Scope<Factory>(new VulkanFactory(
                    desc.applicationName,
                    desc.applicationVersion
                ));
            default:
                return Error("Unknown backend api!");
        }
    }
}
