#include "Entix/Game/Application.h"

#include "Entix/Core/Debug/Logger.h"
#include "Entix/Core/Globals.h"

#include "Entix/Core/Events/Dispatcher.h"

#include "Entix/Core/IO/FileStream.h"
#include "Entix/Core/Tasks/ThreadPool.h"

#include "Entix/RHI/Pipeline.h"
#include "Entix/Resources/HotReloadResourceManager.h"
#include "Entix/Resources/ResourceManager.h"

#include "Platform/Vulkan/VulkanDevice.h"

#include "Platform/SDL/WindowSDL.h"

#include <tracy/Tracy.hpp>

namespace Entix
{
    Application::Application()
        : m_quit(false), m_rhiShader(ResourceId("HELLO"), nullptr)
    {
        // Read config
        {
            IO::FileStream configFile("ApplicationConfig.json", IO::StreamMode::Read);
            JsonArchive configArchive(configFile);

            if(configFile.Exists())
            {
                m_config = ApplicationConfig::Deserialize(configArchive);
            }
            else
            {
                m_config = DefaultOf<ApplicationConfig>();
            }
        }

        // Main application class should have greater priority than anything else.
        EventBus::AddListener(this, EventCategory::Application | EventCategory::Window, 5);

        // Manually initialize a thread pool
        ThreadPool::Instance().Initialize();

        // Initialize the application
        Initialize().Unwrap();
    }

    Application::~Application()
    {
        // Shutdown the application
        Shutdown().Unwrap();

        // Shutdown a thread pool
        ThreadPool::Instance().Shutdown();

        // Unsubscribe from events
        EventBus::RemoveListener(this);

        // Save config
        {
            IO::FileStream configFile("ApplicationConfig.json", IO::StreamMode::Write);
            JsonArchive configArchive(configFile);

            m_config.Serialize(configArchive);
        }
    }

    Result<void> Application::Initialize()
    {
        ZoneScopedN("Application initialization");

        EX_LOG(LogTemp, Info, "Initializing the application.");

        ResourceManager::SetInstance(new HotReloadResourceManager());

        EX_TRY(WSI::Initialize());

        m_window = CreateRef<WindowSDL>(m_config.window);
        m_rhiDevice = CreateRef<VulkanDevice>(m_window.get());
        
        auto resId = ResourceId("Hello");
        EX_LOG(LogTemp, Info, "RESIDTEST: {}", resId.GetFilenameString());

        resId = ResourceId("Hello2");
        EX_LOG(LogTemp, Info, "RESIDTEST2: {}", resId.GetFilenameString());

        m_rhiSwapchain = Ref<RHI::Swapchain>(m_rhiDevice->CreateSwapchain(*m_window).Unwrap());
        m_rhiShader = m_rhiDevice->LoadShader(ResourceId("/home/dmitry/Projects/Entix-Engine/Shaders/SimpleShader.slang"));

        EX_LOG(LogTemp, Info, "Shader Id: {}", m_rhiShader.GetId().GetFilenameString());

        m_rhiGraphicsPipeline = Ref<RHI::GraphicsPipeline>(m_rhiDevice->CreateGraphicsPipeline({}, {m_rhiShader}).Unwrap());

        return {};
    }

    Result<void> Application::Run()
    {
        while(!m_quit)
        {
            EventBus::ProcessEvents();

            WSI::PollEvents();

            FrameMark;
        }

        return {};
    }

    Result<void> Application::Shutdown()
    {
        ZoneScopedN("Application shutdown");

        EX_LOG(LogTemp, Info, "Quit the application.");

        WSI::Shutdown();
        return {};
    }

    void Application::OnEvent(const Event& event)
    {
        EventBus::Dispatch<WindowCloseEvent>(event, [&](const WindowCloseEvent& event) {
            OnWindowCloseRequested(event.GetWindowId());
        });
    }

    void Application::OnWindowCloseRequested([[maybe_unused]] WindowId id) noexcept
    {
        m_quit = true;
    }
}
