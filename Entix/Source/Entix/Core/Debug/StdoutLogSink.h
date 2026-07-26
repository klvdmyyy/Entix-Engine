#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Debug/LogSink.h"

namespace Entix
{
    class ENTIX_API StdoutLogSink : public LogSink
    {
    public:
        void WriteLogEntry(LogEntry entry) override;
    };
}