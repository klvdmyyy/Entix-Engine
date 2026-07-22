#include "Core/EventSystem.h"

#include "Core/Debug/Log.h"

#include <algorithm>

#include <tracy/Tracy.hpp>

EventBus& EventBus::Instance()
{
    static EventBus s_bus;
    return s_bus;
}

void EventBus::SetImmediateMode(bool immediate)
{
    m_immediateMode = immediate;
}

void EventBus::AddListener(EventListener* listener)
{
    m_listeners.push_back(listener);
}

void EventBus::RemoveListener(EventListener* listener)
{
    auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
    if (it != m_listeners.end()) {
        m_listeners.erase(it);
    }
    else {
        /* TODO: Logging here (Trying to remove unexisting listener from event bus) */
    }
}

void EventBus::PublishEvent(const Event& event)
{
    if(m_immediateMode)
        {
            ZoneScopedN("Immediate Frame Processing");
            for(auto listener : m_listeners)
                {
                    listener->OnEvent(event);
                }
            
        }
    else
        {
            std::lock_guard<std::mutex> lock(m_sync);
            m_eventQueue.push(Scope<Event>(event.Clone()));
        }
}

void EventBus::ProcessEvents()
{
    if(m_immediateMode)
        return;

    ZoneScoped;
    
    std::queue<Scope<Event>> current_events;

    {
        std::lock_guard<std::mutex> lock(m_sync);
        std::swap(current_events, m_eventQueue);
    }

    while(!current_events.empty())
        {
            auto& event = *current_events.front();

            for(auto listener : m_listeners)
                {
                    listener->OnEvent(event);
                }

            current_events.pop();
        }
}