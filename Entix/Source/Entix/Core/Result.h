#pragma once

#include "Entix/Core/Base.h"

#include <variant>

namespace Entix
{
    template<typename SuccessType, typename ErrorType>
    class Result
    {
        static_assert(!std::is_same_v<SuccessType, ErrorType>, "SuccessType and ErrorType can't be same in Result class.");

    public:
        EX_FORCE_INLINE
        inline constexpr Result(const SuccessType& value)
            requires (std::is_copy_assignable_v<SuccessType>)
            : m_value(value)
        {
        }

        EX_FORCE_INLINE
        inline constexpr Result(const ErrorType& value)
            requires (std::is_copy_assignable_v<SuccessType>)
            : m_value(value)
        {
        }

        EX_FORCE_INLINE
        inline constexpr Result(SuccessType value)
            requires ((!std::is_copy_assignable_v<SuccessType>) && std::is_move_assignable_v<SuccessType>)
            : m_value(std::move(value))
        {
        }

        EX_FORCE_INLINE
        inline constexpr Result(ErrorType value)
            requires ((!std::is_copy_assignable_v<SuccessType>) && std::is_move_assignable_v<SuccessType>)
            : m_value(std::move(value))
        {
        }

        [[nodiscard]]
        bool IsSuccess() const noexcept
        {
            return std::holds_alternative<SuccessType>(m_value);
        }

        [[nodiscard]]
        bool IsError() const noexcept
        {
            return std::holds_alternative<ErrorType>(m_value);
        }

        [[nodiscard]]
        SuccessType Unwrap() const
            requires (std::is_copy_assignable_v<SuccessType>)
        {
            try {
                return std::get<SuccessType>(m_value);
            } catch ([[maybe_unused]] const std::bad_variant_access& ex) {
                EX_DEBUGBREAK();
                std::exit(1);
            }
        }

        [[nodiscard]]
        SuccessType Unwrap() const
            requires ((!std::is_copy_assignable_v<SuccessType>) && std::is_move_assignable_v<SuccessType>)
        {
            try {
                SuccessType res = std::move(std::get<SuccessType>(m_value));
                m_value = std::monostate{};
                return res;
            } catch ([[maybe_unused]] const std::bad_variant_access& ex) {
                EX_DEBUGBREAK();
                std::exit(1);
            }
        }

        [[nodiscard]]
        ErrorType GetError() const
            requires (std::is_copy_assignable_v<ErrorType>)
        {
            try {
                return std::get<ErrorType>(m_value);
            } catch ([[maybe_unused]] const std::bad_variant_access& ex) {
                EX_DEBUGBREAK();
                std::exit(1);
            }
        }

        [[nodiscard]]
        ErrorType GetError() const
            requires ((!std::is_copy_assignable_v<ErrorType>) && std::is_move_assignable_v<ErrorType>)
        {
            try {
                ErrorType res = std::move(std::get<ErrorType>(m_value));
                m_value = std::monostate{};
                return res;
            } catch ([[maybe_unused]] const std::bad_variant_access& ex) {
                EX_DEBUGBREAK();
                std::exit(1);
            }
        }

    private:
        mutable std::variant<SuccessType, ErrorType, std::monostate> m_value;
    };
}