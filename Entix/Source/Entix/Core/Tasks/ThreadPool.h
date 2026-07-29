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
#include <type_traits>

namespace Entix
{
    class ThreadPool
    {
    public:
        ENTIX_API static ThreadPool& Instance();

        ENTIX_API ThreadPool();
        ENTIX_API ~ThreadPool();

        ThreadPool(ThreadPool&) = delete;
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(ThreadPool&&) = delete;
        ThreadPool& operator=(const ThreadPool&&) = delete;

        ENTIX_API void Initialize(Usize numThreads = std::thread::hardware_concurrency());
        ENTIX_API void Shutdown();

        template<typename F, typename... Args>
        inline auto Enqueue(F&& f, Args&&... args)
        {
            auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            auto encapsulatedPtr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

            auto futureObject = encapsulatedPtr->get_future();
            {
                std::unique_lock<std::mutex> lock(m_sync);
                m_queue.emplace([encapsulatedPtr]() {
                    (*encapsulatedPtr)();
                });
            }
            m_cv.notify_one();
            return futureObject;
        }

    private:

        ENTIX_API void WorkerLoop();

        bool m_initialized;

        std::vector<std::thread> m_workers;
        std::mutex m_sync;
        std::condition_variable m_cv;
        std::queue<std::function<void()>> m_queue;

        bool m_stop;
    };
}