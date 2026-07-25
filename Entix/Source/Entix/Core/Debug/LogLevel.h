#pragma once

#include "Entix/Core/String.h"

namespace Entix
{
    enum class LogLevel
    {
        Trace = 0,
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    ENTIX_API String LogLevelToString(LogLevel level);
}