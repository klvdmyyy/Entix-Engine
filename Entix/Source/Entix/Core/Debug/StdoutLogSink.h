#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Debug/LogSink.h"

#include <mutex>

namespace Entix
{
    class StdoutLogSink : public LogSink
    {
    public:
        ENTIX_API void WriteLogEntry(LogEntry entry) override;

    private:
        std::mutex m_sync;
    };
}