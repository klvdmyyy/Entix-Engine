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
          m_worldContext(m_threadPool, m_resourceManager),
          m_layerStack(&m_controlFlow, &m_worldContext)
    {
        std::vector<RHI::GpuInfo> capableGpuInfos{};

        for(const auto& info : m_rhiFactory->GetSupportedGpuInfos())
        {
            bool isCapable = true;

            // TODO Check GPU Info

            if(isCapable && desc.gpuCapabilitiesCallback(info))
            {
                capableGpuInfos.push_back(info);
            }
        }
        EX_ASSERT_FMT(!capableGpuInfos.empty(), "Supported GPU not found! If you're sure your graphics card is up to the task, try updating your drivers");

        std::ranges::sort(capableGpuInfos.begin(), capableGpuInfos.end(), desc.gpuSortCallback);
        m_renderingDevice.reset(m_rhiFactory->CreateGpuHandle(capableGpuInfos[0]).Unwrap());

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

            for(auto& layer : m_layerStack)
            {
                layer->OnTick();
            }

            for(auto& layer : m_layerStack)
            {
                layer->OnPreRender();
            }

            for(auto& layer : m_layerStack)
            {
                layer->OnRender();
            }

            for(auto& layer : m_layerStack)
            {
                layer->OnPostRender();
            }
        }
    }
}
