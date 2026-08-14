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
        std::vector<RHI::GpuInfo> capableGpus;

        for(const auto& info : m_rhiFactory->GetSupportedGpuInfos())
        {
            bool isCapable = true;

            // TODO Check GPU Info

            if(isCapable && desc.gpuCapabilitiesCallback(info))
            {
                capableGpus.push_back(info);
            }
        }
        EX_ASSERT_FMT(!capableGpus.empty(), "Supported GPU not found! If you're sure your graphics card is up to the task, try updating your drivers");

        std::ranges::sort(capableGpus.begin(), capableGpus.end(), desc.gpuSortCallback);
        m_renderingDevice.reset(m_rhiFactory->CreateGpuHandle(capableGpus[0]).Unwrap());

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
