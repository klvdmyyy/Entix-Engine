#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Debug/LogEntry.h"

namespace Entix
{
    class ENTIX_API LogSink
    {
    public:
        virtual ~LogSink() = default;

        virtual void WriteLogEntry(LogEntry entry) = 0;
    };
}