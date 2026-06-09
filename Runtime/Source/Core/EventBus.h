#pragma once

#include "ERuntimeExport.h"
#include "Core/Event.h"
#include "Core/Memory.h"

#include <algorithm>
#include <vector>
#include <mutex>
#include <memory>
#include <queue>

namespace Core
{
    // Event listener interface
    class EventListener
    {
    public:
        virtual ~EventListener() = default;
        virtual void OnEvent(const Event& event) = 0;
    };

    // Event dispatcher
    class EventDispatcher
    {
    private:
        const Event& event;

    public:
        explicit EventDispatcher(const Event& e) : event(e) {}

        // Dispatch event to handler if types match
        template<typename T, typename F>
        bool Dispatch(const F& handler)
        {
            if (event.GetType() == T::GetStaticType())
            {
                handler(static_cast<const T&>(event));
                return true;
            }
            return false;
        }
    };

    // Event bus
    class ERUNTIME_EXPORT EventBus
    {
    private:
        std::vector<EventListener*> listeners;
        std::queue<Scope<Event>> eventQueue;
        std::mutex queueMutex;
        bool immediateMode = true;

        EventBus() = default;

    public:
        static EventBus& Get();

        EventBus(const EventBus&) = delete;
        EventBus& operator=(const EventBus&) = delete;

        void SetImmediateMode(bool immediate);
        void AddListener(EventListener* listener);
        void RemoveListener(EventListener* listener);
        void PublishEvent(const Event& event);
        void ProcessEvents();
    };
}