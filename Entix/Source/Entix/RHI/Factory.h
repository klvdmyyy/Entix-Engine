/**
 * @file
 * @brief Main RHI class which can manipulate all other classes
 *
 * @defgroup RHI
 */

#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/String.h"
#include "Entix/Core/Memory.h"
#include "Entix/Core/Result.h"
#include "Entix/Core/Version.h"

#include "Entix/WSI/Window.h"

#include <vector>
#include <algorithm>

namespace Entix
{
    namespace RHI
    {
        enum class BackendApi
        {
            Vulkan = 0, //< Default value
        };

        /// @brief Information about your GPU.
        ///
        /// @attention All fields with underscore at the end are not for use. It should be private
        /// but I want to keep GpuInfo as plain data structure.
        struct GpuInfo
        {
            String name;

            /// @details It must be a private member but I want to keep GpuInfo
            /// just plain data structure
            Usize index_;
        };

        struct FactoryDesc
        {
            BackendApi backendApi;
            String applicationName;
            Version applicationVersion;
        };
    }

    template<>
    EX_FORCE_INLINE
    inline RHI::BackendApi DefaultOf<RHI::BackendApi>()
    {
        return RHI::BackendApi::Vulkan;
    }

    template<>
    EX_FORCE_INLINE
    inline RHI::FactoryDesc DefaultOf<RHI::FactoryDesc>()
    {
        using namespace RHI;

        return FactoryDesc {
            .backendApi = DefaultOf<BackendApi>(),
            .applicationName = "No Name",
            .applicationVersion = {0, 1, 0},
        };
    }

    template<>
    EX_FORCE_INLINE
    inline String ToString<RHI::BackendApi>(const RHI::BackendApi& api)
    {
        using RHI::BackendApi;
        switch(api)
        {
            case BackendApi::Vulkan:
                return "Vulkan";

            default:
                return ToString<BackendApi>(DefaultOf<BackendApi>());
        }
    }

    template<>
    EX_FORCE_INLINE
    inline RHI::BackendApi FromString<RHI::BackendApi>(const String& value)
    {
        String backendName = value;
        std::transform(backendName.begin(), backendName.end(), backendName.begin(), [](char c) { return std::tolower(c); });

        using RHI::BackendApi;

        if(backendName == "vulkan") return BackendApi::Vulkan;
        else return DefaultOf<BackendApi>();
    }
}


namespace Entix::RHI
{
    class Device;

    class Factory
    {
    public:
        virtual ~Factory() = default;

        virtual std::vector<GpuInfo> GetSupportedGpuInfos() = 0;

        virtual Result<Device*> CreateGpuHandle(
            const GpuInfo& info,

            // It's used to check Gpu capabilities to render in target window.
            //
            // Notice that all other windows created after main must provide surfaces
            // with same capabilities. You don't need to check Gpu support for all windows you are
            // create, only for main
            //
            //
            // Maybe this parameter should be placed somewhere else but I don't know where for now (:
            Ref<Window> mainWindow = nullptr
        ) = 0;
    };
}
