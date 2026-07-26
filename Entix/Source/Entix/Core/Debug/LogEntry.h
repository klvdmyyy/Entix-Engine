#pragma once

#include "LogCategory.h"

namespace Entix
{
    struct LogEntry
    {
        const LogCategory category;
        const LogLevel level;
        const StringView message;
    };
}