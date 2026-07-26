#pragma once

#include "Entix/Core/Memory.h"

#include "Entix/Core/Debug/LogSink.h"
#include "Entix/Core/IO/Stream.h"

namespace Entix
{
    class StreamLogSink : public LogSink
    {
    public:
        StreamLogSink(Scope<IO::Stream> stream)
            : m_stream(std::move(stream))
        {
        }

        ENTIX_API void WriteLogEntry(LogEntry entry) override;
        
    private:
        Scope<IO::Stream> m_stream;
    };
}