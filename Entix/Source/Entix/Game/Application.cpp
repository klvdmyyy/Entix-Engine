#include "Entix/Game/Application.h"

#include "Entix/Core/Debug/Logger.h"
#include "Entix/Core/Globals.h"

#include "Entix/Core/Events/Dispatcher.h"

#include "Entix/Core/Tasks/ThreadPool.h"

#include "Platform/Vulkan/VulkanDevice.h"
#include "Platform/SDL/WindowSDL.h"

namespace Entix
{
    Application::Application(const ApplicationConfig& config)
        : m_quit(false), k_config(config)
    {
        EventBus::Instance().AddListener(this,
            EventCategory::Application | EventCategory::Window,
            5);
        ThreadPool::Instance().Initialize();
        Initialize().Unwrap();
    }

    Application::~Application()
    {
        Shutdown().Unwrap();
        ThreadPool::Instance().Shutdown();
        EventBus::Instance().RemoveListener(this);
    }

    Result<void> Application::Initialize()
    {
        EX_LOG(LogTemp, Info, "Initializing the application.");

        EX_TRY(WSI::Initialize());

        m_window = CreateRef<WindowSDL>(k_config.window);
        m_rhiDevice = CreateRef<VulkanDevice>();

        auto compiledShader = RHI::ShaderCompiler::Instance()->Compile("/home/dmitry/Projects/Entix-Engine/Shaders/SimpleShader.slang").Unwrap();

        m_rhiShader = Ref<RHI::Shader>(m_rhiDevice->CreateShader(compiledShader).Unwrap());

        return {};
    }

    Result<void> Application::Run()
    {
        while(!m_quit)
        {
            EventBus::Instance().ProcessEvents();

            WSI::PollEvents();
        }

        return {};
    }

    Result<void> Application::Shutdown()
    {
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
