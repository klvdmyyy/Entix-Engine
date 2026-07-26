#pragma once

#include "Entix/Core/Debug/LogSink.h"
#include "Entix/Core/IO/Stream.h"
#include "Entix/Core/IO/TextStream.h"

namespace Entix
{
    class StreamLogSink : public LogSink
    {
    public:
        StreamLogSink(Scope<IO::Stream> stream)
            : m_stream(std::move(stream))
        {
        }

        void WriteLogEntry(LogEntry entry) override
        {
            IO::TextStream::CreateNonOwned(*m_stream).WriteLineFmt("[{}] {}: {}", entry.level, entry.category.GetName(), entry.message);
        }
        
    private:
        Scope<IO::Stream> m_stream;
    };
}