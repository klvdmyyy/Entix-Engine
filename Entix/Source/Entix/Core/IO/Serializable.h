#pragma once

#include <concepts>

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

    // Serializable class must be default initializable.
    //
    // If deserialization target (i.e file) can't be found it just returns default value.
    template<typename T, typename A>
    concept Serializable = (HasSerializeMethod<T, A>
                        && HasDeserializeStaticMethod<T, A>
                        && std::default_initializable<T>)
                        || std::is_trivial_v<T>;
}