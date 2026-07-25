/**
 * @brief Basic Result type used for error-checking
 * 
 * @details
 * > Default error type is `Error` from `Core/Error.h`.
 * 
 * Result type also support void value (`Result<void>`).
 * 
 * ## Why not `std::expected` ?
 * 
 * Because I don't really like how it implemented. That's a reason
 * why I using C++. I just can write my own Result type.
 */
#pragma once

#include "Core/Base.h"
#include "Core/TypeName.h"

#include "Core/Panic.h"
#include "Core/Debug/Log.h"

#include "Core/Error.h"

#include <variant>
#include <optional>
#include <source_location>

/**
 * @brief Main declaration of Result type.
 * 
 * @details
 * > Default `Err` type is `Error` from `Core/Error.h`
 * 
 * Using `std::variant` as a union container for Success/Error values.
 */
template<typename Ok, typename Err = Error>
class Result {
public:
    static_assert(std::is_same<Ok, Err>::value, "Result Success(Ok) and Error(Err) types can't be same!");

    /**
     * @brief Success constructor.
     */
    FORCE_INLINE
    inline constexpr Result(const Ok& value)
        : k_value(value)
    {
    }

    /**
     * @brief Error constructor
     */
    FORCE_INLINE
    inline constexpr Result(const Err& value)
        : k_value(value)
    {
    }

    /**
     * @brief Check if result is success
     * 
     * @return `true` if result is success, otherwise returns `false`
     */
    [[nodiscard]]
    constexpr bool IsOk() const noexcept
    {
        return std::holds_alternative<Ok>();
    }

    /**
     * @brief Check if result is error
     * 
     * @return `true` if result is error, otherwise returns `false`
     */
    [[nodiscard]]
    constexpr bool IsErr() const noexcept
    {
        return std::holds_alternative<Err>();
    }

    /**
     * @brief Unwrap the result.
     */
    [[nodiscard]]
    const Ok& Unwrap(std::source_location location = std::source_location::current()) const
    {
        try {
            return std::get<Ok>(k_value);
        } catch(const std::bad_variant_access& ex) {
            EX_PANIC_FMT_LOC(location, "Unwrap() on error value");
        }
    }

    /**
     * @brief Get the error
     */
    [[nodiscard]]
    const Err& GetError(std::source_location location = std::source_location::current()) const
    {
        try {
            return std::get<Err>(k_value);
        } catch(const std::bad_variant_access& ex) {
            EX_PANIC_FMT_LOC(location, "GetError() on success value");
        }
    }

private:
    const std::variant<Ok, Err> k_value;
};

template<>
class Result<void> {
public:
    FORCE_INLINE
    inline constexpr Result(void)
        : k_error(std::nullopt)
    {
    }

    FORCE_INLINE
    inline constexpr Result(const Error& error)
        : k_error(error)
    {
    }

    [[nodiscard]]
    bool IsOk() const noexcept
    {
        return !k_error.has_value();
    }

    [[nodiscard]]
    bool IsErr() const noexcept
    {
        return k_error.has_value();
    }

    [[nodiscard]]
    const Error& GetError(std::source_location location = std::source_location::current()) const
    {
        if(IsErr())
        {
            return k_error.value();
        }
        else
        {
            EX_PANIC_FMT_LOC(location, "GetError() on success value");
        }
    }

private:
    const std::optional<Error> k_error;
};