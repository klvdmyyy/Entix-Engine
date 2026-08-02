#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Result.h"
#include "Entix/Core/Memory.h"

#include "Entix/Core/Events/Listener.h"

#include "Entix/WSI/Base.h"
#include "Entix/WSI/Window.h"

#include "Entix/RHI/Pipeline.h"
#include "Entix/RHI/Device.h"
#include "Entix/RHI/Shader.h"
#include "Entix/RHI/Swapchain.h"

#include "Entix/Serialization/JsonArchive.h"

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

        void Serialize(JsonArchive& ar)
        {
            ar & AField("window", window)
               & AField("graphicsApi", graphicsApi);
        }

        static ApplicationConfig Deserialize(JsonArchive& ar)
        {
            ApplicationConfig config;

            ar & AField("window", config.window)
               & AField("graphicsApi", config.graphicsApi);

            return config;
        }
    };

    class Application : EventListener
    {
    public:
        ENTIX_API Application();
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
        ApplicationConfig m_config;

        Ref<Window> m_window;
        Ref<RHI::Device> m_rhiDevice;
        ResourceHandle<RHI::Shader> m_rhiShader;
        Ref<RHI::Swapchain> m_rhiSwapchain;
        Ref<RHI::GraphicsPipeline> m_rhiGraphicsPipeline;
    };
}