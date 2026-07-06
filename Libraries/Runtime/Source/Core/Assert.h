// -*- mode: c++; -*-
#pragma once

#include "Core/Debug/Log.h"

#include <print>
#include <format>
#include <cstdlib>

#ifndef NDEBUG
#define EX_ASSERT(expr, fmt, ...)                                       \
    do {                                                                \
        if (!(expr)) {                                                  \
            EX_LOG(Critical, ::LogCategory::Core,                       \
                   "Assertion '{}' failed. Message: {}", #expr,         \
                   std::format(fmt,##__VA_ARGS__));                     \
            std::exit(1);                                               \
        }                                                               \
    } while (0)
#else
#define EX_ASSERT(expr, ...) do { expr; } while(0)
#endif