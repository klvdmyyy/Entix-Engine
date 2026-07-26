#pragma once

#include <concepts>
#include <type_traits>

namespace Entix::IO
{
    template<typename T, typename A>
    concept HasSerializeMethod = requires(T& obj, A& ar)
    {
        { obj.Serialize(ar) } -> std::same_as<void>;
    };

    template<typename T, typename A>
    concept HasDeserializeStaticMethod = requires(T& obj, A& ar)
    {
        { T::Deserialize(ar) } -> std::same_as<T>;
    };

    template<typename T, typename A>
    concept Serializable = HasSerializeMethod<T, A> && HasDeserializeStaticMethod<T, A>;
}