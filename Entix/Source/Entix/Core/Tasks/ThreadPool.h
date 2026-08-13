#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Types.h"
#include "Entix/Core/Result.h"

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <future>

namespace Entix
{
    class ThreadPool
    {
    public:
        static Usize GetOptimalWorkerCount();

        ENTIX_API ThreadPool(Usize workerCount = GetOptimalWorkerCount());
        ENTIX_API ~ThreadPool();

        ThreadPool(ThreadPool&) = delete;
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(ThreadPool&&) = delete;
        ThreadPool& operator=(const ThreadPool&&) = delete;

        template<typename F, typename... Args>
        inline auto Enqueue(F&& f, Args&&... args)
        {
            auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            auto encapsulatedPtr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

            auto futureObject = encapsulatedPtr->get_future();
            {
                std::unique_lock lock(m_sync);
                m_queue.emplace([encapsulatedPtr]() {
                    (*encapsulatedPtr)();
                });
            }
            m_cv.notify_one();
            return futureObject;
        }

        template<typename F, typename... Args>
        inline void EnqueueToMainThread(F&& f, Args&&... args)
        {
            auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            {
                std::unique_lock lock(m_mainThreadSync);
                m_queue.emplace(func);
            }
        }

        ENTIX_API void ExecuteMainThreadQueue();

    private:
        ENTIX_API void WorkerLoop();

        bool m_initialized;

        std::queue<std::function<void()>> m_mainThreadQueue;
        std::mutex m_mainThreadSync;

        std::vector<std::thread> m_workers;
        std::queue<std::function<void()>> m_queue;
        std::mutex m_sync;
        std::condition_variable m_cv;

        bool m_stop;
    };
}
