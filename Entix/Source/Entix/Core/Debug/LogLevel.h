#pragma once

#include "Entix/Core/String.h"

#include <ranges>
#include <sstream>
#include <format>

namespace Entix
{
    enum class LogLevel
    {
        Trace = 0,
        Debug,
        Info,
        Warning,
        Error,
        Critical,

        Count
    };

    ENTIX_API String LogLevelToString(LogLevel level);
}

template<>
struct std::formatter<::Entix::LogLevel, char>
{
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
        auto it = ctx.begin();
        return it;
    }

    template<typename FmtContext>
    FmtContext::iterator format(::Entix::LogLevel level,
                                FmtContext &ctx) const
    {
        std::ostringstream out;
        out << ::Entix::LogLevelToString(level);

        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};