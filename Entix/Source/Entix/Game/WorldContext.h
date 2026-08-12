#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Memory.h"
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
            ResourceManager& resourceManager,
            const Ref<RHI::Device>& renderingDevice
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
        inline RHI::Device& GetRenderingDevice() const noexcept { return *m_renderingDevice; }

    private:
        ThreadPool& m_threadPool;
        ResourceManager& m_resourceManager;

        Ref<RHI::Device> m_renderingDevice;
    };
}
