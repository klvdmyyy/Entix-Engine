#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Events/Listener.h"
#include "Entix/Core/Memory.h"

#include <concepts>
#include <cstring>
#include <mutex>
#include <queue>
#include <type_traits>

namespace Entix
{
    class EventBus
    {
    public:
        EX_FORCE_INLINE
        inline static void AddListener(
            EventListener* listener,
            EventCategory categoryFilter,
            Int32 priority = 0
        )
        {
            AddListener(listener, static_cast<Int32>(categoryFilter), priority);
        }

        ENTIX_API static void AddListener(
            EventListener* listener,
            Int32 categoryFilter = static_cast<Int32>(EventCategory::None),
            Int32 priority = 0
        );
        ENTIX_API static void RemoveListener(EventListener* listener);

        ENTIX_API static void PublishEvent(const Event& event);

        template<std::derived_from<Event> E, typename F>
            requires (std::is_invocable_v<F, const E&>)
        static void Dispatch(const Event& event, const F& f)
        {
            if(strcmp(event.GetType(), E::GetStaticType()) == 0)
            {
                std::invoke(f, static_cast<const E&>(event));
            }
        }

        ENTIX_API static void ProcessEvents();

        ENTIX_API static void SetImmediateMode(bool enabled) noexcept;
        ENTIX_API static bool GetImmediateMode() noexcept;

    private:
        struct ListenerInfo
        {
            EventListener* listener;
            Int32 categoryFilter;
            Int32 priority;
        };

        inline static std::vector<ListenerInfo> m_listeners;
        inline static std::queue<Scope<Event>> m_eventQueue;
        inline static std::mutex m_queueMutex;

        inline static bool m_immediateMode;
    };
}