#include "Entix/Core/Events/Dispatcher.h"

#include "Entix/Core/Globals.h"
#include "Entix/Core/Debug/Logger.h"

#include <algorithm>
#include <mutex>

namespace Entix
{
    void EventBus::AddListener(EventListener* listener, Int32 categoryFilter, Int32 priority)
    {
        m_listeners.push_back({.listener = listener, .categoryFilter = categoryFilter, .priority = priority});

        std::sort(m_listeners.begin(), m_listeners.end(), [](const auto& a, const auto& b) {
            return a.priority > b.priority;
        });
    }

    void EventBus::RemoveListener(EventListener* listener)
    {
        auto it = std::ranges::find_if(m_listeners.begin(), m_listeners.end(), [listener](const ListenerInfo& info) {
            return listener == info.listener;
        });

        if(it != m_listeners.end())
        {
            m_listeners.erase(it);
        }
        else
        {
            EX_LOG(LogTemp, Warning, "Trying to remove unexisting event listener!");
        }
    }

    void EventBus::PublishEvent(const Event& event)
    {
        if(m_immediateMode)
        {
            for(auto& info : m_listeners)
            {
                if(info.categoryFilter == static_cast<Int32>(EventCategory::None) || (event.GetCategoryFlags() & info.categoryFilter))
                {
                    info.listener->OnEvent(event);
                }
            }
        }
        else
        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            m_eventQueue.push(Scope<Event>(event.Clone()));
        }
    }

    void EventBus::ProcessEvents()
    {
        std::queue<Scope<Event>> currentEvents;

        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            std::swap(currentEvents, m_eventQueue);
        }

        while(!currentEvents.empty())
        {
            auto& event = *currentEvents.front();

            for(auto& info : m_listeners)
            {
                if(info.categoryFilter == static_cast<Int32>(EventCategory::None) || (event.GetCategoryFlags() & info.categoryFilter))
                {
                    info.listener->OnEvent(event);
                }
            }

            currentEvents.pop();
        }
    }

    void EventBus::SetImmediateMode(bool enabled) noexcept
    {
        if(enabled)
            ProcessEvents();

        m_immediateMode = enabled;
    }

    bool EventBus::GetImmediateMode() noexcept
    {
        return m_immediateMode;
    }
}