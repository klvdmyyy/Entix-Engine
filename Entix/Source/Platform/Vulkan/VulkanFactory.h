#pragma once

#include "Entix/Core/Base.h"
#include "Entix/RHI/Factory.h"

namespace Entix
{
    class VulkanFactory final : public RHI::Factory
    {
    public:
        ENTIX_API VulkanFactory(const String& applicationName, Version applicationVersion);
        ENTIX_API ~VulkanFactory();

        std::vector<RHI::GpuInfo> GetSupportedGpuInfos() override { return {}; };
        Result<RHI::Device*> CreateGpuHandle(const RHI::GpuInfo& info) override { (void)info; return nullptr; }

    private:
        const String k_applicationName;
        const Version k_applicationVersion;
    };
}
