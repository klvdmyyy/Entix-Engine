#include "Entix/Core/Events/Dispatcher.h"

namespace Entix
{
    EventBus& EventBus::Instance()
    {
        static EventBus s_instance;
        return s_instance;
    }

    void EventBus::SetImmediateMode(bool enabled)
    {
        m_immediateMode = enabled;
        if(enabled)
        {
            ProcessEvents();
        }
    }

    bool EventBus::IsImmediateMode() const noexcept
    {
        return m_immediateMode;
    }

    void EventBus::ClearQueue() noexcept
    {
        while(!m_eventQueue.empty())
        {
            m_eventQueue.pop();
        }
    }

    void EventBus::ProcessEvents()
    {
        while(!m_eventQueue.empty())
        {
            auto& queued = m_eventQueue.front();
            queued.Dispatch();
            m_eventQueue.pop();
        }
    }
}