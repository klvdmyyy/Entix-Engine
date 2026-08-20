/**
 * @file
 * @brief Unrecovarable errors (Panic)
 *
 * @addtogroup Core
 */
#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/String.h"

#include <source_location>

namespace Entix
{
    /**
     * @brief Throw unrecovarable error.
     *
     * @param[in] location Source location. Used only in debug purposes, you shouldn't override default value!
     * @throws std::runtime_error Runtime error which are catched by entry point and closes application after that.
     */
    ENTIX_API void Panic [[noreturn]] (std::source_location location = std::source_location::current());

    /**
     * @brief Throw unrecovarable error with message.
     *
     * @param[in] message This message will be saved in logs
     * @param[in] location Source location. Used only in debug purposes, you shouldn't override default value!
     * @throws std::runtime_error Runtime error which are catched by entry point and closes application after that.
     */
    ENTIX_API void Panic [[noreturn]] (StringView message, std::source_location location = std::source_location::current());
}
