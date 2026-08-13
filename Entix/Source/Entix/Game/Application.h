#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Tasks/ThreadPool.h"

#include "Entix/Resources/ResourceManager.h"

#include "Entix/RHI/Factory.h"
#include "Entix/RHI/Device.h"

#include "Entix/Game/WorldContext.h"
#include "Entix/Game/ControlFlow.h"

namespace Entix
{
    struct [[nodiscard]] ApplicationDesc
    {
        Usize threads = 0;
        bool enableHotReload = false;

        RHI::FactoryDesc rhiFactoryDesc = DefaultOf<RHI::FactoryDesc>();

        // This is necessary if the developer wants to add their own GPU requirements
        std::function<bool(const RHI::GpuInfo& gpuInfo)> gpuCapabilitiesCallback = [](const auto& info) { (void)info; return true; };
    };

    class Application
    {
    public:
        ENTIX_API Application(const ApplicationDesc& desc = DefaultOf<ApplicationDesc>());
        ENTIX_API ~Application();

        // Unable to copy
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        // Unable to move
        Application(Application&&) = delete;
        Application& operator=(Application&&) = delete;

        void Run();

    private:
        ThreadPool m_threadPool;
        ResourceManager m_resourceManager;

        Scope<RHI::Factory> m_rhiFactory;
        Ref<RHI::Device> m_renderingDevice;

        WorldContext m_worldContext;
        ControlFlow m_controlFlow;
    };
}
