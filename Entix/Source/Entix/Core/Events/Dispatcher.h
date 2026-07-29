#pragma once

#include "Entix/Core/Events/Handler.h"
#include "Entix/Core/Memory.h"

#include <vector>
#include <typeindex>
#include <unordered_map>
#include <queue>

namespace Entix
{
    template<EventCategory Category>
    class EventDispatcher
    {
    public:
        template<typename E, typename Handler>
            requires EventInCategory<E, Category> && EventHandler<Handler, E>
        void Subscribe(Handler&& handler)
        {
            std::function<void(const void*)> wrapper = [callable = std::forward<Handler>(handler)](const void* rawEvent)
            {
                const E& event = *static_cast<const E*>(rawEvent);
                if constexpr (std::is_invocable_v<Handler, const E&>) {
                    std::invoke(callable, event);
                }
                else if constexpr (std::is_invocable_v<Handler, const E*>) {
                    std::invoke(callable, &event);
                }
                else {
                    std::invoke(callable);
                }
            };

            m_handlers[std::type_index(typeid(E))].push_back(std::move(wrapper));
        }

        template<typename E>
            requires EventInCategory<E, Category>
        void Subscribe(std::function<void(const E&)> callback)
        {
            std::function<void(const void*)> wrapper = [callback = std::move(callback)](const void* rawEvent) {
                callback(*static_cast<const E*>(rawEvent));
            };
            m_handlers[std::type_index(typeid(E))].push_back(std::move(wrapper));
        }

        template<typename E>
            requires EventInCategory<E, Category>
        void Send(const E& event)
        {
            auto it = m_handlers.find(std::type_index(typeid(E)));

            if(it == m_handlers.end())
                return;
            
            for(const auto& wrapper : it->second)
            {
                wrapper(&event);
            }
        }

        void Clear()
        {
            m_handlers.clear();
        }

        template<typename E>
            requires EventInCategory<E, Category>
        void ClearForEvent()
        {
            m_handlers.erase(std::type_index(typeid(E)));
        }
    
    private:
        using HandlerWrapper = std::function<void(const void*)>;

        std::unordered_map<std::type_index, std::vector<HandlerWrapper>> m_handlers;
    };

    struct QueuedEvent
    {
        std::type_index Type;
        std::function<void()> Dispatch;
    };

    class EventBus
    {
    public:
        ENTIX_API static EventBus& Instance();

        ENTIX_API void SetImmediateMode(bool enabled);
        ENTIX_API bool IsImmediateMode() const noexcept;
        ENTIX_API void ClearQueue() noexcept;
        ENTIX_API void ProcessEvents();

        template<typename Category>
            requires EventCategory<Category> || std::is_same_v<Category, DefaultEventCategory>
        EventDispatcher<Category>& GetDispatcher()
        {
            static EventDispatcher<Category> dispatcher;
            return dispatcher;
        }

        template<typename E>
        void Send(const E& event)
        {
            using Category = EventCategoryOf<E>;

            if(IsImmediateMode())
            {
                GetDispatcher<Category>().Send(event);
            }
            else
            {
                QueueEvent<E>(event);
            }
        }

        template<typename E, typename Handler>
            requires EventHandler<Handler, E>
        void Subscribe(Handler&& handler)
        {
            using Category = EventCategoryOf<E>;
            GetDispatcher<Category>().template Subscribe<E>(std::forward<Handler>(handler));
        }

        template<typename E>
        void Subscribe(std::function<void(const E&)> callback) {
            using Category = EventCategoryOf<E>;
            GetDispatcher<Category>().template Subscribe<E>(std::move(callback));
        }


    private:
        EventBus() = default;

        template<typename E>
        void QueueEvent(const E& event)
        {
            using Category = EventCategoryOf<E>;

            auto eventPtr = CreateRef<E>(event);

            auto dispatchFunc = [this, eventPtr]()
            {
                GetDispatcher<Category>().Send(*eventPtr);
            };

            m_eventQueue.push(QueuedEvent{
                .Type = std::type_index(typeid(E)),
                .Dispatch = dispatchFunc
            });
        }

        std::queue<QueuedEvent> m_eventQueue;
        bool m_immediateMode = false;
    };
}