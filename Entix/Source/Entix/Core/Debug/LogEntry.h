#pragma once

#include "LogCategory.h"

namespace Entix
{
    struct LogEntry
    {
        const LogLevel level;
        const LogCategory category;
        const StringView message;
    };
}