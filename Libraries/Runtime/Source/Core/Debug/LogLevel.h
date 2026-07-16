// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/String.h"

#include <sstream>
#include <format>

enum class LogLevel {
    Trace = 0,
    Info,
    Warning,
    Error,
    Critical,
};

[[nodiscard]]
ERUNTIME_API const StringView LogLevelToString(const LogLevel& level);

template<>
struct ERUNTIME_API std::formatter<LogLevel, char>
{
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
        auto it = ctx.begin();
        return it;
    }

    template<typename FmtContext>
    FmtContext::iterator format(LogLevel level,
                                FmtContext &ctx) const
    {
        std::ostringstream out;
        out << LogLevelToString(level);

        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};