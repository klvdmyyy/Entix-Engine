/**
 * @file
 * @brief Result type for error-handling
 * 
 * @details
 * Rust-inspired Result class for error-handling.
 * 
 * @ingroup Core
 */

#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Error.h"

#include <variant>

/**
 * @brief Try to execute the resulting expression
 * 
 * @details
 * Usage Example:
 * @code
 * extern Result<void> AnotherResultingFn();
 * 
 * Result<void> SomeFn()
 * {
 *      EX_TRY(AnotherResultingFn());
 *      return {};
 * }
 * @endcode
 * 
 * @ingroup Code
 */
#define EX_TRY(EXPR) \
    if(auto res = EXPR; res.IsError()) \
        return res.UnwrapErr()

/**
 * @brief Try to execute the resulting expression and if it success save unwrapped result to variable.
 * 
 * @details
 * Usage Example:
 * @code
 * extern Result<int> AnotherResultingFn();
 * 
 * int SomeFn()
 * {
 *      EX_LET_TRY(code, AnotherResultingFn());
 *      return code; // It returns `AnotherResultingFn()` result if it successfuly executed
 * }
 * @endcode
 * 
 * @ingroup Core
 */
#define EX_LET_TRY(VAR, EXPR) \
    auto VAR##__entix_try_result__ = EXPR; \
    if(VAR##__entix_try_result__.IsError()) \
        return VAR##__entix_try_result__.UnwrapErr(); \
    auto VAR = VAR##__entix_try_result__.Unwrap()

namespace Entix
{
    template<typename SuccessType, typename ErrorType = Error>
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
            requires (std::is_copy_assignable_v<ErrorType>)
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
            requires ((!std::is_copy_assignable_v<ErrorType>) && std::is_move_assignable_v<ErrorType>)
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

        explicit operator bool() const noexcept
        {
            return IsSuccess();
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
        ErrorType UnwrapErr() const
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
        ErrorType UnwrapErr() const
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

    template<typename ErrorType>
    class Result<void, ErrorType>
    {
    public:
        EX_FORCE_INLINE
        inline constexpr Result(void)
            : m_value(std::monostate{})
        {
        }

        EX_FORCE_INLINE
        inline constexpr Result(const ErrorType& value)
            requires (std::is_copy_assignable_v<ErrorType>)
            : m_value(value)
        {
        }

        EX_FORCE_INLINE
        inline constexpr Result(ErrorType value)
            requires ((!std::is_copy_assignable_v<ErrorType>) && std::is_move_assignable_v<ErrorType>)
            : m_value(std::move(value))
        {
        }

        [[nodiscard]]
        bool IsSuccess() const noexcept
        {
            return !std::holds_alternative<ErrorType>(m_value);
        }

        [[nodiscard]]
        bool IsError() const noexcept
        {
            return std::holds_alternative<ErrorType>(m_value);
        }

        operator bool() const noexcept
        {
            return IsSuccess();
        }

        [[nodiscard]]
        ErrorType UnwrapErr() const
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
        ErrorType UnwrapErr() const
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
        mutable std::variant<ErrorType, std::monostate> m_value;
    };
}