// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/Panic.h"

#include <print>
#include <format>
#include <cstdlib>
#include <filesystem>

#ifndef NDEBUG

// Usual assertion macro. Always evaluate expression.
//
// (but throws breakpoint only in debug mode. no checks in release mode)
#define EX_ASSERT(expr, fmt, ...)                                       \
    do {                                                                \
        if (!(expr)) {                                                  \
            Panic(std::format(fmt,##__VA_ARGS__));                      \
            EX_DEBUGBREAK();                                            \
        }                                                               \
    } while (0)

#define EX_ASSERT_EXPR(expr) EX_ASSERT(expr, "Expression assert!")

// Debug assertion macro. Expression is evaluated only in Debug build.
//
// In debug build just same as usual EX_ASSERT assertion macro!
#define EX_DEBUG_ASSERT(expr, fmt, ...) EX_ASSERT(expr, fmt,##__VA_ARGS__)

#define EX_DEBUG_ASSERT_EXPR(expr) EX_ASSERT_EXPR(expr)

#else

#define EX_ASSERT(expr, ...) do { expr; } while(0)
#define EX_ASSERT_EXPR(expr) EX_ASSERT(expr, "Expression assert!")

#define EX_DEBUG_ASSERT(expr, fmt, ...) do {} while(0)
#define EX_DEBUG_ASSERT_EXPR(expr) do {} while(0)

#endif