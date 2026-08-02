#include "Entix/Core/Tasks/ThreadPool.h"

#include "Entix/Core/Globals.h"
#include "Entix/Core/Debug/Logger.h"

namespace Entix
{
    ThreadPool& ThreadPool::Instance()
    {
        static ThreadPool s_instance;
        return s_instance;
    }

    ThreadPool::ThreadPool()
        : m_initialized(false), m_stop(false)
    {
    }

    ThreadPool::~ThreadPool()
    {
    }

    void ThreadPool::Initialize(Usize numThreads)
    {
        if(m_initialized) return;
        else m_initialized = true;

        EX_LOG(ThreadPool, Info, "Initializing thread pool with {} threads.", numThreads);

        m_stop = false;
        for(Usize i = 0; i < numThreads; i++)
        {
            m_workers.emplace_back(&ThreadPool::WorkerLoop, this);
        }
    }

    void ThreadPool::Shutdown()
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