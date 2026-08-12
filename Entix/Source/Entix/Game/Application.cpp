#include "Entix/Game/Application.h"

#include "Entix/Core/Events/Dispatcher.h"

#include <tracy/Tracy.hpp>

namespace Entix
{
    Application::Application(const ApplicationDesc& desc)
        : m_threadPool(desc.threads == 0 ? std::thread::hardware_concurrency() : desc.threads),
          m_resourceManager(m_threadPool, desc.enableHotReload),
          m_worldContext(m_threadPool, m_resourceManager)
    {
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while(!m_controlFlow.IsQuitRequested())
        {
            EventBus::ProcessEvents();

            m_threadPool.ExecuteMainThreadQueue();
        }
    }
}
