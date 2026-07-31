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
        ENTIX_API static EventBus& Instance();

        EX_FORCE_INLINE
        inline void AddListener(
            EventListener* listener,
            EventCategory categoryFilter,
            Int32 priority = 0
        )
        {
            AddListener(listener, static_cast<Int32>(categoryFilter), priority);
        }

        ENTIX_API void AddListener(
            EventListener* listener,
            Int32 categoryFilter = static_cast<Int32>(EventCategory::None),
            Int32 priority = 0
        );
        ENTIX_API void RemoveListener(EventListener* listener);

        ENTIX_API void PublishEvent(const Event& event);

        template<std::derived_from<Event> E, typename F>
            requires (std::is_invocable_v<F, const E&>)
        static void Dispatch(const Event& event, const F& f)
        {
            if(strcmp(event.GetType(), E::GetStaticType()) == 0)
            {
                std::invoke(f, static_cast<const E&>(event));
            }
        }

        ENTIX_API void ProcessEvents();

        ENTIX_API void SetImmediateMode(bool enabled) noexcept;
        ENTIX_API bool GetImmediateMode() const noexcept;

    private:
        struct ListenerInfo
        {
            EventListener* listener;
            Int32 categoryFilter;
            Int32 priority;
        };

        std::vector<ListenerInfo> m_listeners;
        std::queue<Scope<Event>> m_eventQueue;
        std::mutex m_queueMutex;

        bool m_immediateMode;
    };
}