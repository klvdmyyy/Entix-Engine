#include "Core/EventBus.h"

namespace Core
{
    EventBus& EventBus::Get()
    {
        static EventBus s_Bus;
        return s_Bus;
    }

    void EventBus::SetImmediateMode(bool immediate)
    {
        immediateMode = immediate;
    }

    void EventBus::AddListener(EventListener* listener)
    {
        listeners.push_back(listener);
    }

    void EventBus::RemoveListener(EventListener* listener)
    {
        auto it = std::find(listeners.begin(), listeners.end(), listener);
        if (it != listeners.end()) {
            listeners.erase(it);
        }
        else {
            /* TODO: Logging here (Trying to remove unexisting listener from event bus) */
        }
    }

    void EventBus::PublishEvent(const Event& event)
    {
        if (immediateMode) {
            // Dispatch event immediately
            for (auto listener : listeners) {
                listener->OnEvent(event);
            }
        }
        else {
            // Queue event for later processing
            std::lock_guard<std::mutex> lock(queueMutex);
            eventQueue.push(Scope<Event>(event.Clone()));
        }
    }

    void EventBus::ProcessEvents()
    {
        if (immediateMode) return;

        std::queue<std::unique_ptr<Event>> currentEvents;

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            std::swap(currentEvents, eventQueue);
        }

        while (!currentEvents.empty()) {
            auto& event = *currentEvents.front();

            for (auto listener : listeners) {
                listener->OnEvent(event);
            }

            currentEvents.pop();
        }
    }
}