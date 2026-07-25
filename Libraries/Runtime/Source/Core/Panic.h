#pragma once

#include "Core/Base.h"
#include "Core/String.h"

#include <source_location>
#include <format>

#define EX_PANIC() Panic()
#define EX_PANIC_FMT(fmt, ...) Panic(std::format(fmt,##__VA_ARGS__))

#define EX_PANIC_LOC(location) Panic(location)
#define EX_PANIC_FMT_LOC(location, fmt, ...) Panic(std::format(fmt,##__VA_ARGS__), location)

void Panic(StringView message, std::source_location location = std::source_location::current());

FORCE_INLINE
inline void Panic(std::source_location location = std::source_location::current())
{
    Panic("NO MESSAGE", location);
}
