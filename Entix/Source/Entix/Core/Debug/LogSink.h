#pragma once

#include "Entix/Core/Debug/LogEntry.h"

namespace Entix
{
    class LogSink
    {
    public:
        virtual ~LogSink() = default;

        virtual void WriteLogEntry(LogEntry entry) = 0;
    };
}