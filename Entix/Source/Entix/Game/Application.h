#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Result.h"
#include "Entix/Core/Memory.h"

#include "Entix/WSI/Base.h"
#include "Entix/WSI/Window.h"

#include "Entix/RHI/Device.h"

namespace Entix
{
    /**
     * @todo Serialization through IO::Archive
     */
    struct ApplicationConfig
    {
        ApplicationConfig() = default;

        WindowConfig window;
        RHI::GraphicsApi graphicsApi = RHI::GraphicsApi::Vulkan;
    };

    class Application
    {
    public:
        ENTIX_API Application(const ApplicationConfig& config);
        ENTIX_API ~Application();

        ENTIX_API Result<void> Run();

        /** Events */

        ENTIX_API void OnWindowCloseRequested(WindowId id) noexcept;

    private:
        ENTIX_API Result<void> Initialize();
        ENTIX_API Result<void> Shutdown();

        bool m_quit;
        const ApplicationConfig k_config;

        Ref<Window> m_window;
        Ref<RHI::Device> m_rhiDevice;
    };
}