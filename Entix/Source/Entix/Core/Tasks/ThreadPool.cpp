#include "Entix/Core/Tasks/ThreadPool.h"

#include "Entix/Core/Globals.h"
#include "Entix/Core/Debug/Logger.h"

namespace Entix
{
    Usize ThreadPool::GetOptimalWorkerCount()
    {
        return std::thread::hardware_concurrency() - 1;
    }

    ThreadPool::ThreadPool(Usize workerCount)
        : m_initialized(false), m_stop(false)
    {
        if(m_initialized) return;
        else m_initialized = true;

        EX_LOG(ThreadPool, Info, "Initializing thread pool with {}+1(main) threads.", workerCount);

        m_stop = false;
        for(Usize i = 0; i < workerCount; i++)
        {
            m_workers.emplace_back(&ThreadPool::WorkerLoop, this);
        }
    }

    ThreadPool::~ThreadPool()
    {
        if(!m_initialized) return;

        EX_LOG(ThreadPool, Info, "Shutdown the thread pool.");

        {
            std::unique_lock<std::mutex> lock(m_sync);
            m_stop = true;
        }

        m_cv.notify_all();
        for(auto& worker : m_workers)
        {
            worker.join();
        }
    }

    void ThreadPool::ExecuteMainThreadQueue()
    {
        std::queue<std::function<void()>> localQueue;

        {
            std::unique_lock lock(m_mainThreadSync);
            std::swap(localQueue, m_mainThreadQueue);
        }

        while(!localQueue.empty())
        {
            auto func = localQueue.front();
            func();
            localQueue.pop();
        }
    }


    void ThreadPool::WorkerLoop()
    {
        for(;;)
        {
            std::function<void()> currentTask;
            {
                std::unique_lock<std::mutex> lock(m_sync);
                m_cv.wait(lock, [this]() {
                    return m_stop || !m_queue.empty();
                });

                if(m_stop && m_queue.empty()) break;
                if(m_queue.empty()) continue;

                currentTask = m_queue.front();
                m_queue.pop();
            }
            currentTask();
        }
    }
}
