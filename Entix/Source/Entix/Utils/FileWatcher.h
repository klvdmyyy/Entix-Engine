#pragma once

#include "Entix/Core/Result.h"
#include "Entix/Core/Tasks/ThreadPool.h"

#include <filesystem>
#include <functional>
#include <mutex>

namespace Entix
{
    class FileWatcher
    {
    public:
        using CallbackFn = std::function<void(const std::filesystem::path&)>;

        FileWatcher(ThreadPool& threadPool, CallbackFn&& callback);
        ~FileWatcher();

        Result<void> Emplace(const std::filesystem::path& path);
        Result<void> Erase(const std::filesystem::path& path);

    private:
        void WatcherThread();

        ThreadPool& m_threadPool;
        CallbackFn m_callback;

        std::unordered_map<std::filesystem::path, std::filesystem::file_time_type> m_fileTimestamps;
        std::mutex m_sync;

        bool m_running = false;
        std::thread m_watcherThread;
    };
}
