#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Tasks/ThreadPool.h"

#include "Entix/Game/LayerStack.h"
#include "Entix/Resources/ResourceManager.h"

#include "Entix/RHI/Factory.h"
#include "Entix/RHI/Device.h"

#include "Entix/Game/WorldContext.h"
#include "Entix/Game/ControlFlow.h"

namespace Entix
{
    struct ApplicationDesc
    {
        int argc = 0;
        char** argv = nullptr;

        String name = "No Name";
        String description = "No Description";
        Version version = {0, 1, 0};

        Usize threads = 0;
        bool enableHotReload = false;

        RHI::FactoryDesc rhiFactoryDesc = DefaultOf<RHI::FactoryDesc>();

        // Application choosing first capable GPU from array
        std::function<bool(const RHI::GpuInfo&, const RHI::GpuInfo&)> gpuSortCallback = [](const RHI::GpuInfo& a, const RHI::GpuInfo& b) { return a.index_ > b.index_; };

        // This is necessary if the developer wants to add their own GPU requirements
        std::function<bool(const RHI::GpuInfo&)> gpuCapabilitiesCallback = [](const auto& info) { (void)info; return true; };
    };

    class ApplicationDescBuilder
    {
    public:
        ApplicationDescBuilder() = default;
        ~ApplicationDescBuilder() = default;

        EX_FORCE_INLINE
        inline ApplicationDescBuilder& SetCLIArgs(int argc, char** argv)
        {
            m_desc.argc = argc;
            m_desc.argv = argv;

            return *this;
        }

        EX_FORCE_INLINE
        inline ApplicationDescBuilder& SetName(const String& name)
        {
            m_desc.name = name;
            m_desc.rhiFactoryDesc.applicationName = name;

            return *this;
        }

        EX_FORCE_INLINE
        inline ApplicationDescBuilder& SetDescription(const String& desc)
        {
            m_desc.description = desc;

            return *this;
        }

        EX_FORCE_INLINE
        inline ApplicationDescBuilder& SetVersion(const Version& version)
        {
            m_desc.version = version;
            m_desc.rhiFactoryDesc.applicationVersion = version;

            return *this;
        }

        EX_FORCE_INLINE
        inline ApplicationDescBuilder& SetRHIBackendAPI(const RHI::BackendApi& api)
        {
            m_desc.rhiFactoryDesc.backendApi = api;

            return *this;
        }

        EX_FORCE_INLINE
        inline ApplicationDescBuilder& SetThreadCount(Usize count) noexcept
        {
            m_desc.threads = count;

            return *this;
        }

        EX_FORCE_INLINE
        inline ApplicationDescBuilder& EnableResourceHotReload() noexcept
        {
            m_desc.enableHotReload = true;

            return *this;
        }

        EX_FORCE_INLINE
        inline ApplicationDescBuilder& SetGPUSortFunction(std::function<bool(const RHI::GpuInfo&, const RHI::GpuInfo&)> callback)
        {
            m_desc.gpuSortCallback = callback;

            return *this;
        }

        EX_FORCE_INLINE
        inline ApplicationDescBuilder& SetGPUCapabilitiesCallback(std::function<bool(const RHI::GpuInfo&)> callback)
        {
            m_desc.gpuCapabilitiesCallback = callback;

            return *this;
        }

        EX_FORCE_INLINE
        inline Result<ApplicationDesc> Build() { return m_desc; }

    private:
        ApplicationDesc m_desc{};
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

        LayerStack m_layerStack;
    };
}
