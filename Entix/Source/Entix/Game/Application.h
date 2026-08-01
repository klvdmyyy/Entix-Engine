#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Result.h"
#include "Entix/Core/Memory.h"

#include "Entix/Core/Events/Listener.h"

#include "Entix/RHI/Pipeline.h"
#include "Entix/WSI/Base.h"
#include "Entix/WSI/Window.h"

#include "Entix/RHI/Device.h"
#include "Entix/RHI/Shader.h"
#include "Entix/RHI/Swapchain.h"

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

    class Application : EventListener
    {
    public:
        ENTIX_API Application(const ApplicationConfig& config);
        ENTIX_API ~Application();

        // Unable to copy
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        // Unable to move
        Application(Application&&) = delete;
        Application& operator=(Application&&) = delete;

        ENTIX_API Result<void> Run();

        /** Events */

        ENTIX_API void OnEvent(const Event& event) final;
        ENTIX_API void OnWindowCloseRequested(WindowId id) noexcept;

    private:
        ENTIX_API Result<void> Initialize();
        ENTIX_API Result<void> Shutdown();

        bool m_quit;
        const ApplicationConfig k_config;

        Ref<Window> m_window;
        Ref<RHI::Device> m_rhiDevice;
        ResourceHandle<RHI::Shader> m_rhiShader;
        Ref<RHI::Swapchain> m_rhiSwapchain;
        Ref<RHI::GraphicsPipeline> m_rhiGraphicsPipeline;
    };
}