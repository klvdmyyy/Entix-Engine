#include "Entix/Game/Application.h"

#include "Entix/Core/Debug/Logger.h"
#include "Entix/Core/Globals.h"

#include "Platform/Vulkan/VulkanDevice.h"

namespace Entix
{
    Application::Application()
        : m_quit(false)
    {
        m_rhiDevice = CreateRef<VulkanDevice>();
    }

    Application::~Application()
    {
    }

    Result<void> Application::Initialize()
    {
        EX_LOG(LogTemp, Info, "Initializing the application.");
        return {};
    }

    Result<void> Application::Run()
    {
        EX_TRY(Initialize());

        // while(!m_quit)
        // {
        // }

        EX_TRY(Shutdown());

        return {};
    }

    Result<void> Application::Shutdown()
    {
        EX_LOG(LogTemp, Info, "Quit the application.");
        return {};
    }
}