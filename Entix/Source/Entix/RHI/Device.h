#pragma once

namespace Entix::RHI
{
    enum class GraphicsApi
    {
        Vulkan = 0, //< Default option
    };

    class Device
    {
    public:
        virtual ~Device() = default;
    };
}