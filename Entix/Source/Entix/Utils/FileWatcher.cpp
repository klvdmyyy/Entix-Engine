#include "Entix/Utils/FileWatcher.h"

#include "Entix/Core/Globals.h"
#include "Entix/Core/Debug/Logger.h"

#include <filesystem>

namespace Entix
{
    FileWatcher::FileWatcher(ThreadPool& threadPool, CallbackFn&& callback)
        : m_threadPool(threadPool),
          m_callback(callback)
    {
        m_running = true;
        m_watcherThread = std::thread(&FileWatcher::WatcherThread, this);
    }

    FileWatcher::~FileWatcher()
    {
        m_running = false;
        if(m_watcherThread.joinable())
        {
            m_watcherThread.join();
        }
    }

    Result<void> FileWatcher::Emplace(const std::filesystem::path& path)
    {
        try
        {
            std::unique_lock lock(m_sync);
            m_fileTimestamps[path] = std::filesystem::last_write_time(path);
        } catch(const std::filesystem::filesystem_error& err)
        {
            return Error("Failed to watch path");
        }

        return {};
    }

    Result<void> FileWatcher::Erase(const std::filesystem::path& path)
    {
        std::unique_lock lock(m_sync);

        auto it = m_fileTimestamps.find(path);

        if(it == m_fileTimestamps.end())
        {
            return Error("Path not found");
        }

        m_fileTimestamps.erase(it);
        return {};
    }

    void FileWatcher::WatcherThread()
    {
        while(m_running)
        {
            for(auto& [path, timestamp] : m_fileTimestamps)
            {
                // TODO Maybe we need to put unique_lock to `try` or `if` statement ?!
                std::unique_lock lock(m_sync);

                try
                {
                    auto currentTimestamp = std::filesystem::last_write_time(path);
                    if(currentTimestamp != timestamp)
                    {
                        // Execute callback function in main thread!
                        m_threadPool.EnqueueToMainThread(m_callback, path);

                        timestamp = currentTimestamp;
                    }
                }
                catch(const std::filesystem::filesystem_error& err)
                {
                    EX_LOG(LogUtils, Warning, "Failed to watch resource file '{}'. What: {}", path.string(), err.what());
                }
            }
        }
    }
}
