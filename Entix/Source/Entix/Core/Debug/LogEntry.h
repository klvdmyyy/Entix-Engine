#pragma once

#include "LogCategory.h"

#include <chrono>

namespace Entix
{
    struct LogEntry
    {
        using Clock = std::chrono::utc_clock;

        Clock::time_point time;
        const LogCategory category;
        const LogLevel level;
        const StringView message;
    };
}