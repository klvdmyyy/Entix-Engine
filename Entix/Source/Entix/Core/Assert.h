#pragma once

#ifdef ENTIX_BUILD_DEBUG
#    include "Entix/Core/Panic.h"
#    define EX_ASSERT(EXPR)                                                                                            \
        do                                                                                                             \
        {                                                                                                              \
            if (!(EXPR))                                                                                               \
            {                                                                                                          \
                Panic(std::format("Assertion failed: {}", #EXPR));                                                     \
            }                                                                                                          \
        } while (0)

/**
 * @brief Assertion with custom message
 *
 * @param[in] EXPR Expression to check
 * @param[in] FMT Format string
 * @param[in] ... Format args (if necessary)
 *
 * @throws std::runtime_error See @verbatim embed:rst:inline {cpp:func}`Entix::Panic` @endverbatim
 *
 * @details
 * Usage example:
 * @code{.cpp}
 * void SoUnstableThing()
 * {
 *     EX_ASSERT_FMT(false, "Hello, {}!", "World");
 * }
 * @endcode
 */
#    define EX_ASSERT_FMT(EXPR, FMT, ...)                                                                              \
        do                                                                                                             \
        {                                                                                                              \
            if (!(EXPR))                                                                                               \
            {                                                                                                          \
                Panic(std::format("Assertion failed: {} Message: {}", #EXPR, std::format(FMT, ##__VA_ARGS__)));        \
            }                                                                                                          \
        } while (0)
#else
#    define EX_ASSERT(EXPR)                                                                                            \
        do                                                                                                             \
        {                                                                                                              \
            (void)(EXPR);                                                                                                      \
        } while (0)
#    define EX_ASSERT_FMT(EXPR, ...)                                                                                   \
        do                                                                                                             \
        {                                                                                                              \
            (void)(EXPR);                                                                                                      \
        } while (0)
#endif
