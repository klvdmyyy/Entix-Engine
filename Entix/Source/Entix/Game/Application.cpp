#include "Entix/Game/Application.h"

#include "Entix/Core/Assert.h"
#include "Entix/Core/Events/Dispatcher.h"

#include <tracy/Tracy.hpp>

namespace Entix
{
    Application::Application(const ApplicationDesc& desc)
        : m_threadPool(desc.threads == 0 ? ThreadPool::GetOptimalWorkerCount() : desc.threads),
          m_resourceManager(m_threadPool, desc.enableHotReload),
          m_rhiFactory(RHI::Factory::Create(desc.rhiFactoryDesc).Unwrap()),
          m_renderingDevice(nullptr),
          m_worldContext(m_threadPool, m_resourceManager)
    {
        for(const auto& info : m_rhiFactory->GetSupportedGpuInfos())
        {
            bool isCapable = true;

            // TODO Check GPU Info

            if(isCapable && desc.gpuCapabilitiesCallback(info))
            {
                m_renderingDevice.reset(m_rhiFactory->CreateGpuHandle(info).Unwrap());
                break;
            }
        }
        EX_ASSERT_FMT(m_renderingDevice, "Supported GPU not found! If you're sure your graphics card is up to the task, try updating your drivers");

        m_worldContext.SetRenderingDevice(m_renderingDevice);
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
