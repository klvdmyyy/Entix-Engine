#pragma once

#include "Entix/Core/Panic.h"

#ifdef ENTIX_BUILD_DEBUG
#    define EX_ASSERT(EXPR)                                                                                            \
        do                                                                                                             \
        {                                                                                                              \
            if (!(EXPR))                                                                                               \
            {                                                                                                          \
                Panic(std::format("Assertion failed: {}", #EXPR));                                                     \
            }                                                                                                          \
        } while (0)
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
            EXPR                                                                                                       \
        } while (0)
#    define EX_ASSERT_FMT(EXPR, ...)                                                                                   \
        do                                                                                                             \
        {                                                                                                              \
            EXPR                                                                                                       \
        } while (0)
#endif