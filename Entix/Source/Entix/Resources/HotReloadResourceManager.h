#pragma once

#include "Entix/Resources/ResourceManager.h"

#include <filesystem>
#include <thread>

namespace Entix
{
    class HotReloadResourceManager : public ResourceManager
    {
    public:
        ENTIX_API static HotReloadResourceManager& Instance();

        ENTIX_API HotReloadResourceManager();
        ENTIX_API ~HotReloadResourceManager();

        ENTIX_API void StartWatcher();
        ENTIX_API void StopWatcher();

        template<std::derived_from<Resource> T>
        ResourceHandle<T> Load(const ResourceId& resourceId)
        {
            auto handle = ResourceManager::Load<T>(resourceId);

            try
            {
                m_fileTimestamps[resourceId] = std::filesystem::last_write_time(resourceId.GetFilepath());
            }
            catch(const std::filesystem::filesystem_error& e)
            {
                EX_LOG(Resources, Warning, "Failed to watch resource file: '{}'", resourceId.GetFilenameString());
            }

            return handle;
        }

    private:
        ENTIX_API void WatcherThread();
        ENTIX_API void ReloadResource(const ResourceId& resourceId);

        std::unordered_map<ResourceId, std::filesystem::file_time_type, ResourceId::Hasher> m_fileTimestamps;
        std::thread m_watcherThread;
        bool m_running = false;
    };
}