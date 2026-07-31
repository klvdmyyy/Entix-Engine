#include "Entix/Game/Application.h"

#include "Entix/Core/Debug/Logger.h"
#include "Entix/Core/Globals.h"

#include "Entix/Core/Events/Dispatcher.h"

#include "Entix/Core/Tasks/ThreadPool.h"

#include "Platform/Vulkan/VulkanDevice.h"

#include "Platform/SDL/WindowSDL.h"

#include <tracy/Tracy.hpp>

namespace Entix
{
    Application::Application(const ApplicationConfig& config)
        : m_quit(false), k_config(config)
    {
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
    }

    Result<void> Application::Initialize()
    {
        ZoneScopedN("Application initialization");

        EX_LOG(LogTemp, Info, "Initializing the application.");

        EX_TRY(WSI::Initialize());

        m_window = CreateRef<WindowSDL>(k_config.window);
        m_rhiDevice = CreateRef<VulkanDevice>(m_window.get());

        m_rhiSwapchain = Ref<RHI::Swapchain>(m_rhiDevice->CreateSwapchain(*m_window).Unwrap());

        auto compiledShader = RHI::ShaderCompiler::Instance()->Compile("/home/dmitry/Projects/Entix-Engine/Shaders/SimpleShader.slang").Unwrap();

        m_rhiShader = Ref<RHI::Shader>(m_rhiDevice->CreateShader(compiledShader).Unwrap());

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
