#pragma once

#include "Entix/Core/Events/Event.h"
#include "Entix/Core/Result.h"

#include <functional>

namespace Entix
{
    template<typename H, typename E>
    concept EventHandler = requires(H& handler, const E& event)
    {
        { handler.Handle(event) } -> std::convertible_to<void>;
    } || requires(H& handler, const E& event)
    {
        { handler(event) } -> std::convertible_to<void>;
    };

    template<typename H, typename E, typename R>
    concept EventHandlerWithResult = requires(H& handler, const E& event)
    {
        { handler.Handle(event) } -> std::convertible_to<R>;
    } || requires(H& handler, const E& event)
    {
        { handler(event) } -> std::convertible_to<R>;
    };

    template<typename E>
    class EventHandlerBase
    {
    public:
        virtual ~EventHandlerBase() = default;
        virtual void Handle(const E& event) = 0;
    };

    template<typename E, typename Callable>
        requires EventHandler<Callable, E>
    class EventHandlerWrapper : public EventHandlerBase<E>
    {
    public:
        explicit EventHandlerWrapper(Callable&& callable)
            : m_callable(std::forward<Callable>(callable)) {}

        void Handle(const E& event) override
        {
            if constexpr(std::is_invocable_v<Callable, const E&>)
            {
                std::invoke(m_callable, event);
            }
            else if constexpr(std::is_invocable_v<Callable, const E*>)
            {
                std::invoke(m_callable, &event);
            }
            else
            {
                std::invoke(m_callable);
            }
        }

    private:
        Callable m_callable;
    };
}