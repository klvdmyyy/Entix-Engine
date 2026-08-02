#pragma once

#include <cstdint>
#include <cstddef>

#include <type_traits>
#include <any>

#define EX_DEFINE_ID_TYPE(NAME) \
    struct NAME##Tag {}; \
    using NAME = ::Entix::Id<::Entix::Usize, NAME##Tag>

namespace Entix
{
    using Int8 = int8_t;
    using Int16 = int16_t;
    using Int32 = int32_t;
    using Int64 = int64_t;

    using Uint8 = uint8_t;
    using Uint16 = uint16_t;
    using Uint32 = uint32_t;
    using Uint64 = uint64_t;

    using Usize = size_t;

    using Any = std::any;

    template<typename T, typename Tag>
        requires (std::is_trivial_v<T> && std::is_unsigned_v<T> && std::is_empty_v<Tag>)
    class Id {
    public:
        inline constexpr Id(T value)
            : m_value(value)
        {
        }

        inline constexpr T Get() const noexcept { return m_value; }

    private:
        T m_value;
    };
}