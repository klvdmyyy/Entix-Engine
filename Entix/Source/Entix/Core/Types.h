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

    /**
     * @brief Id template type.
     *
     * @details
     * Usage example:
     * @code{.cpp}
     * struct MyIdTag {};
     * struct MyIdTag2 {};
     *
     * void IdTest()
     * {
     *      Id<Usize, MyIdTag> myId = 10;
     *      Id<Usize, MyIdTag2> myId2 = 10;
     *
     *      // This is error. You can't compare different Id types implicitly
     *      if(myId == myId2)
     *      {
     *          // Do something...
     *      }
     *
     *      // Following convertion are acceptable.
     *      //
     *      // Convertion are available when Id type
     *      // are convertable or the same (std::is_convertible_v || std::is_same_v)
     *      if(myId == (Id<Usize, MyIdTag>)myId2)
     *      {
     *          // Do something...
     *      }
     * }
     * @endcode
     *
     * Following code doing the same but with macro:
     * @code{.cpp}
     * EX_DEFINE_ID_TYPE(MyId);
     * EX_DEFINE_ID_TYPE(MyId2);
     *
     * void IdTest()
     * {
     *      MyId myId = 10;
     *      MyId2 myId2 = 10;
     *
     *      // This is error. You can't compare different Id types implicitly
     *      if(myId == myId2)
     *      {
     *          // Do something...
     *      }
     *
     *      // Following convertion are acceptable.
     *      //
     *      // Convertion are available when Id type
     *      // are convertable or the same (std::is_convertible_v || std::is_same_v)
     *      if(myId == (MyId)myId2)
     *      {
     *          // Do something...
     *      }
     * }
     * @endcode
     */
    template<typename T, typename Tag>
        requires (std::is_trivial_v<T> && std::is_unsigned_v<T> && std::is_empty_v<Tag>)
    class Id {
    public:
        inline constexpr Id(T value)
            : m_value(value)
        {
        }

        inline constexpr T Get() const noexcept { return m_value; }

        inline T& operator*() noexcept
        {
            return m_value;
        }

        inline bool operator==(const Id<T, Tag>& other) const noexcept
        {
            return m_value == other.m_value;
        }

        inline bool operator!=(const Id<T, Tag>& other) const noexcept
        {
            return !(*this == other);
        }

        template<typename OtherT, typename OtherTag>
            requires (std::is_same_v<T, OtherT> || std::is_convertible_v<T, OtherT>)
        inline explicit operator Id<OtherT, OtherTag>() const noexcept
        {
            return Id<OtherT, OtherTag>(static_cast<OtherT>(m_value));
        }

        /**
         * @brief Hasher struct for Id type.
         *
         * @details
         * Usage example:
         * @code{.cpp}
         * // Type definitions
         * EX_DEFINE_ID_TYPE(PlayerId);
         * struct Player {};
         *
         * // Somewhere in your class or function
         * std::unordered_map<PlayerId, Player, PlayerId::Hasher> players;
         * @endcode
         */
        struct Hasher
        {
            Usize operator()(const Id<T, Tag>& id)
            {
                return std::hash<T>{}(id.m_value);
            }
        };

    private:
        T m_value;
    };
}
