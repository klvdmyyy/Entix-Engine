#pragma once

#include "Entix/Resources/ResourceManager.h"

#include <chrono>
#include <filesystem>
#include <thread>

namespace Entix
{
    class HotReloadResourceManager : public ResourceManager
    {
    public:
        HotReloadResourceManager() { StartWatcher(); }
        ~HotReloadResourceManager() { StopWatcher(); }

        void StartWatcher()
        {
            if(m_running) return;

            m_running = true;
            m_watcherThread = std::thread(&HotReloadResourceManager::WatcherThread, this);
        }

        void StopWatcher()
        {
            if(!m_running) return;

            m_running = false;
            if(m_watcherThread.joinable())
            {
                m_watcherThread.join();
            }
        }

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
        void WatcherThread()
        {
            while(m_running)
            {
                for(auto& [resourceId, timestamp] : m_fileTimestamps)
                {
                    try
                    {
                        auto currentTimestamp = std::filesystem::last_write_time(resourceId.GetFilepath());
                        if(currentTimestamp != timestamp)
                        {
                            ReloadResource(resourceId);
                            timestamp = currentTimestamp;
                        }
                    }
                    catch(const std::filesystem::filesystem_error& e)
                    {
                        EX_LOG(Resources, Warning, "Failed to watch resource file: '{}'", resourceId.GetFilenameString());
                    }
                }

                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }

        void ReloadResource(const ResourceId& resourceId)
        {
            (void)resourceId;
        }

        std::unordered_map<ResourceId, std::filesystem::file_time_type, ResourceId::Hasher> m_fileTimestamps;
        std::thread m_watcherThread;
        bool m_running = false;
    };
}