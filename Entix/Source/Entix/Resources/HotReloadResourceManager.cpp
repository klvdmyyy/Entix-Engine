#include "Entix/Resources/HotReloadResourceManager.h"

#include "Entix/Core/Events/Dispatcher.h"

#include <chrono>

#include <tracy/Tracy.hpp>

namespace Entix
{
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
        ZoneScopedN("Resource reloading");
        ZoneTextF("%s", resourceId.GetFilepathString().c_str());

        EX_LOG(Resources, Info, "Changes detected. Reloading resource: '{}'", resourceId.GetFilenameString());

        EventBus::PublishEvent(ResourceReloadedEvent(resourceId));
    }

    void HotReloadResourceManager::LoadMiddleware(const ResourceId& resourceId)
    {
        try
        {
            m_fileTimestamps[resourceId] = std::filesystem::last_write_time(resourceId.GetFilepath());
        }
        catch(const std::filesystem::filesystem_error& e)
        {
            EX_LOG(Resources, Warning, "Failed to watch resource file: '{}'", resourceId.GetFilenameString());
        }
    }
}
