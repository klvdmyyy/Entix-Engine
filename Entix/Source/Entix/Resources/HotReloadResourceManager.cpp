#include "Entix/Resources/HotReloadResourceManager.h"

#include <chrono>

namespace Entix
{
    HotReloadResourceManager& HotReloadResourceManager::Instance()
    {
        static HotReloadResourceManager s_instance;
        return s_instance;
    }

    HotReloadResourceManager::HotReloadResourceManager() { StartWatcher(); }
    HotReloadResourceManager::~HotReloadResourceManager() { StopWatcher(); }

    void HotReloadResourceManager::StartWatcher()
    {
        if(m_running) return;

        m_running = true;
        m_watcherThread = std::thread(&HotReloadResourceManager::WatcherThread, this);
    }

    void HotReloadResourceManager::StopWatcher()
    {
        if(!m_running) return;

        m_running = false;
        if(m_watcherThread.joinable())
        {
            m_watcherThread.join();
        }
    }

    void HotReloadResourceManager::WatcherThread()
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

    void HotReloadResourceManager::ReloadResource(const ResourceId& resourceId)
    {
        (void)resourceId;
    }
}