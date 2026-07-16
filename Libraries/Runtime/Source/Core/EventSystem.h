#pragma once

#include "Core/Memory.h"

#include <mutex>
#include <queue>
#include <type_traits>
#include <vector>

/* Macro to help define event types */
#define DEFINE_EVENT_TYPE(type)                                         \
    static const char* GetStaticType() { return #type; }                \
    virtual const char* GetType() const override { return GetStaticType(); } \
    virtual ::Event* Clone() const override { return new type(*this); }

class ERUNTIME_API Event
    {
    public:
    virtual ~Event() = default;

    virtual const char* GetType() const = 0;
    virtual Event* Clone() const = 0;
    };

class ERUNTIME_API EventListener
    {
    public:
    virtual ~EventListener() = default;
    virtual void OnEvent(const Event& event) = 0;
    };

class ERUNTIME_API EventDispatcher
    {
    public:
    explicit EventDispatcher(const Event& e) : m_event(e)
    {
    }

    template<std::derived_from<Event> T, typename F>
    bool Dispatch(const F& handler)
    {
        if (m_event.GetType() == T::GetStaticType())
            {
                handler(static_cast<const T &>(m_event));
                return true;
            }
        return false;
    }

    private:
    const Event& m_event;
    };

class ERUNTIME_API EventBus
    {
    public:
    static EventBus& Instance();

    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    void SetImmediateMode(bool immediate);
    void AddListener(EventListener* listener);
    void RemoveListener(EventListener* listener);
    void PublishEvent(const Event& event);
    void ProcessEvents();

    private:
    std::vector<EventListener*> m_listeners;
    std::queue<Scope<Event>> m_eventQueue;
    std::mutex m_sync;

    bool m_immediateMode = true;

    EventBus() = default;
    };
    
template<std::derived_from<Event> T, typename F>
FORCE_INLINE
inline void DispatchEvent(const Event& event, F fn)
{
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<T>(fn);
}