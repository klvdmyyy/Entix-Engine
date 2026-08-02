#pragma once

#include "Entix/Core/Base.h"

#include <concepts>
#include <string>

namespace Entix
{
    using String = std::string;
    using StringView = std::string_view;

    template<typename T>
    String ToString(const T& obj);

    template<typename T>
    T FromString(const String& str);

    template<typename T>
    T DefaultOf();

    template<typename T>
    concept HasDefaultStaticMethod = requires ()
    {
        { T::Default() } -> std::same_as<T>;
    };

    template<typename T>
    concept DefaultOfImplemented = requires ()
    {
        { DefaultOf<T>() } -> std::same_as<T>;
    };

    template<typename T>
    concept HasDefaults = HasDefaultStaticMethod<T>
                       || std::default_initializable<T>
                       || DefaultOfImplemented<T>;

    template<typename T>
    concept ToStringImplemented = requires (const T& obj)
    {
        { ToString(obj) } -> std::same_as<String>;
    };

    template<typename T>
    concept HasToStringMethod = requires (const T& obj)
    {
        { obj.ToString() } -> std::same_as<String>;
    };

    template<typename T>
    concept ConvertibleToString = std::convertible_to<T, String>
                               || HasToStringMethod<T>
                               || ToStringImplemented<T>;

    template<typename T, typename F>
    concept HasFromStaticMethodFor = requires (const F& obj)
    {
        { T::From(obj) } -> std::same_as<T>;
    };

    template<typename T>
    concept FromStringImplemented = requires (const String& obj)
    {
        { FromString<T>(obj) } -> std::same_as<T>;
    };

    template<typename T>
    concept ConstructibleFromString = (std::constructible_from<T, String>
                                   || HasFromStaticMethodFor<T, String>
                                   || FromStringImplemented<T>)
                                   && HasDefaults<T>;

    // Косвенно строка.
    //
    // Концепт который предполагает возможность конвертации
    // объект <-> строка.
    template<typename T>
    concept IndirectString = ConvertibleToString<T> && ConstructibleFromString<T>;

    template<typename T>
        requires (std::default_initializable<T>)
    T DefaultOf()
    {
        return T{};
    }

    template<typename T>
        requires (HasDefaultStaticMethod<T>)
    T DefaultOf()
    {
        return T::Default();
    }

    template<typename T>
        requires (HasToStringMethod<T>)
    String ToString(const T& obj)
    {
        return obj.ToString();
    }

    template<typename T>
        requires (HasFromStaticMethodFor<T, String>)
    T FromString(const String& str)
    {
        return T::From(str);
    }

    template<typename T>
        requires (std::convertible_to<T, String>)
    String ToString(const T& obj)
    {
        return (String)obj;
    }

    template<typename T>
        requires (std::constructible_from<T, String>)
    T FromString(const String& str)
    {
        return T{str};
    }
}