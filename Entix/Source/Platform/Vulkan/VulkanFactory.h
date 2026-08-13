#pragma once

#include "Entix/RHI/Factory.h"

namespace Entix
{
    class VulkanFactory final : public RHI::Factory
    {
    public:
        VulkanFactory(const String& applicationName, Version applicationVersion);
        ~VulkanFactory();

        std::vector<RHI::GpuInfo> GetSupportedGpuInfos() override { return {}; };
        Result<RHI::Device*> CreateGpuHandle(const RHI::GpuInfo& info) override { (void)info; return nullptr; }
    };
}
