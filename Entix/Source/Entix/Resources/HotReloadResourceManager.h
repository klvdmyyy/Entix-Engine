#pragma once

#include "Entix/Resources/ResourceManager.h"

#include <filesystem>
#include <thread>

namespace Entix
{
    class HotReloadResourceManager : public ResourceManager
    {
    public:
        ENTIX_API HotReloadResourceManager();
        ENTIX_API ~HotReloadResourceManager();

        ENTIX_API void StartWatcher();
        ENTIX_API void StopWatcher();

        ENTIX_API void LoadMiddleware(const ResourceId& resourceId) final;

    private:
        ENTIX_API void WatcherThread();
        ENTIX_API void ReloadResource(const ResourceId& resourceId);

        std::unordered_map<ResourceId, std::filesystem::file_time_type, ResourceId::Hasher> m_fileTimestamps;
        std::thread m_watcherThread;
        bool m_running = false;
    };
}