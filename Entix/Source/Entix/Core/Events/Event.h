#pragma once

#include "Entix/Core/String.h"

#include <concepts>
#include <type_traits>

#define DEFINE_EVENT_CATEGORY(NAME) struct NAME {}

namespace Entix
{
    // Concepts

    template<typename T>
    concept EventCategory = std::is_empty_v<T> && std::is_trivial_v<T>;

    DEFINE_EVENT_CATEGORY(DefaultEventCategory);

    template<EventCategory... Categories>
    struct EventCategoryGroup {};

    template<typename E, typename = void>
    struct EventCategoryTrait
    {
        using Type = DefaultEventCategory;
    };

    template<typename E>
    struct EventCategoryTrait<E, std::void_t<typename E::Category>>
    {
        using Type = typename E::Category;
    };

    template<typename E>
    using EventCategoryOf = typename EventCategoryTrait<E>::Type;

    template<typename E, typename C>
    concept EventInCategory = std::is_same_v<EventCategoryOf<E>, C>;

    template<typename Derived>
    class Event {
    public:
        using Category = EventCategoryOf<Derived>;

        String GetName() const noexcept
        {
            return typeid(Derived).name();
        }

        template<EventCategory C>
        constexpr bool IsCategory() const
        {
            return std::is_same_v<C, EventCategoryOf<Derived>>;
        }
    };
}