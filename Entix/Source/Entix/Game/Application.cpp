#include "Entix/Game/Application.h"

#include "Entix/Core/Debug/Logger.h"
#include "Entix/Core/Globals.h"

#include "Entix/Core/Events/Dispatcher.h"

#include "Platform/Vulkan/VulkanDevice.h"
#include "Platform/SDL/WindowSDL.h"

namespace Entix
{
    Application::Application(const ApplicationConfig& config)
        : m_quit(false), k_config(config)
    {
    }

    Application::~Application()
    {
    }

    Result<void> Application::Initialize()
    {
        EX_LOG(LogTemp, Info, "Initializing the application.");

        auto& bus = EventBus::Instance();

        bus.Subscribe<WindowCloseEvent>([&](const WindowCloseEvent& event) {
            OnWindowCloseRequested(event.id);
        });

        EX_TRY(WSI::Initialize());

        m_window = CreateRef<WindowSDL>(k_config.window);
        m_rhiDevice = CreateRef<VulkanDevice>();

        auto compiledShader = RHI::ShaderCompiler::Instance()->Compile("../../Shaders/SimpleShader.slang").Unwrap();

        m_rhiShader = Ref<RHI::Shader>(m_rhiDevice->CreateShader(compiledShader).Unwrap());

        return {};
    }

    Result<void> Application::Run()
    {
        EX_TRY(Initialize());

        while(!m_quit)
        {
            EventBus::Instance().ProcessEvents();

            WSI::PollEvents();
        }

        EX_TRY(Shutdown());

        return {};
    }

    Result<void> Application::Shutdown()
    {
        EX_LOG(LogTemp, Info, "Quit the application.");
        WSI::Shutdown();
        return {};
    }

    void Application::OnWindowCloseRequested([[maybe_unused]] WindowId id) noexcept
    {
        m_quit = true;
    }
}