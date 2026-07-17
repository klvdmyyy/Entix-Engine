// -*- mode: c++; -*-
#pragma once

#include "Core/Debug/Log.h"

#include <print>
#include <format>
#include <cstdlib>

#ifndef NDEBUG

// Usual assertion macro. Always evaluate expression.
//
// (but throws breakpoint only in debug mode. no checks in release mode)
#define EX_ASSERT(expr, fmt, ...)                                       \
    do {                                                                \
        if (!(expr)) {                                                  \
            EX_LOG(Critical, ::LogCategory::Core,                       \
                   "Assertion '{}' failed. Message: {}", #expr,         \
                   std::format(fmt,##__VA_ARGS__));                     \
            std::exit(1);                                               \
        }                                                               \
    } while (0)

// Debug assertion macro. Expression is evaluated only in Debug build.
//
// In debug build just same as usual EX_ASSERT assertion macro!
#define EX_DEBUG_ASSERT(expr, fmt, ...) EX_ASSERT(expr, fmt,##__VA_ARGS__)

#else

#define EX_ASSERT(expr, ...) do { expr; } while(0)
#define EX_DEBUG_ASSERT(expr, fmt, ...) do {} while(0)

#endif