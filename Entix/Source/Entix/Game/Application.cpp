#include "Entix/Game/Application.h"

#include "Entix/Core/Assert.h"
#include "Entix/Core/Events/Dispatcher.h"

#include "Platform/SDL/SDLFactory.h"

#include <tracy/Tracy.hpp>

namespace Entix
{
    Application::Application(const ApplicationDesc& desc)
        : m_threadPool(desc.threads == 0 ? ThreadPool::GetOptimalWorkerCount() : desc.threads),
          m_resourceManager(m_threadPool, desc.enableHotReload),
          m_sdlFactory(CreateScope<SDLFactory>()),
          m_mainWindow(nullptr),
          m_rhiFactory(m_sdlFactory->CreateRHIFactory(desc.rhiFactoryDesc).Unwrap()),
          m_renderingDevice(nullptr),
          m_worldContext(m_threadPool, m_resourceManager),
          m_layerStack(&m_controlFlow, &m_worldContext)
    {
        EventBus::AddListener(this, EventCategory::Application | EventCategory::Window, 15);

        EX_LOG(LogTemp, Info, "Creating a main window.");
        m_mainWindow.reset(m_sdlFactory->CreateWindow({}).Unwrap());

        EX_LOG(LogTemp, Info, "Finding capable GPU for rendering.");
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

        EX_LOG(LogTemp, Debug, "Sorting capable GPUs.");
        std::ranges::sort(capableGpuInfos.begin(), capableGpuInfos.end(), desc.gpuSortCallback);
        
        EX_LOG(LogTemp, Info, "Found capable GPU for rendering: {}", capableGpuInfos[0].name);
        m_renderingDevice.reset(m_rhiFactory->CreateGpuHandle(capableGpuInfos[0]).Unwrap());

        m_worldContext.SetRenderingDevice(m_renderingDevice);
    }

    Application::~Application()
    {
        EX_LOG(LogTemp, Info, "Quiting application.");
        EventBus::RemoveListener(this);
    }

    void Application::OnEvent(const Event& event)
    {
        EventBus::Dispatch<WindowCloseEvent>(event, [this](const WindowCloseEvent& event) {
            this->OnWindowCloseRequested(event.GetWindowId());
        });
    }

    void Application::OnWindowCloseRequested([[maybe_unused]] WindowId windowId)
    {
        m_controlFlow.Quit();
    }

    void Application::Run()
    {
        EX_LOG(LogTemp, Info, "Running application.");

        while(!m_controlFlow.IsQuitRequested())
        {
            EventBus::ProcessEvents();
            m_threadPool.ExecuteMainThreadQueue();

            m_sdlFactory->PollEvents();

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
