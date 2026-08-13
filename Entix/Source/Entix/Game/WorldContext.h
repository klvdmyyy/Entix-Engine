#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Memory.h"
#include "Entix/Core/Assert.h"
#include "Entix/Core/Tasks/ThreadPool.h"

#include "Entix/RHI/Device.h"
#include "Entix/Resources/ResourceManager.h"


namespace Entix
{
    class WorldContext
    {
    public:
        WorldContext(
            ThreadPool& threadPool,
            ResourceManager& resourceManager
        );
        ~WorldContext() = default;

        // Not copyable
        WorldContext(const WorldContext&) = delete;
        WorldContext& operator=(const WorldContext&) = delete;

        // Not movable
        WorldContext(WorldContext&&) = delete;
        WorldContext& operator=(WorldContext&&) = delete;

        EX_FORCE_INLINE
        inline ThreadPool& GetThreadPool() const noexcept { return m_threadPool; }

        EX_FORCE_INLINE
        inline ResourceManager& GetResourceManager() const noexcept { return m_resourceManager; }

        EX_FORCE_INLINE
        inline RHI::Device& GetRenderingDevice() const noexcept
        {
            EX_ASSERT(m_renderingDevice);
            return *m_renderingDevice;
        }

    protected:
        friend class Application;

        void SetRenderingDevice(const Ref<RHI::Device>& renderingDevice) noexcept
        {
            m_renderingDevice = renderingDevice;
        }

    private:
        ThreadPool& m_threadPool;
        ResourceManager& m_resourceManager;

        Ref<RHI::Device> m_renderingDevice;
    };
}
