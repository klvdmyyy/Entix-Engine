#include "Entix/Core/Debug/StreamLogSink.h"

#include "Entix/Core/IO/TextStream.h"

namespace Entix
{
    void StreamLogSink::WriteLogEntry(LogEntry entry)
    {
        IO::TextStream::CreateNonOwned(*m_stream)
            .WriteLineFmt("{0:%F}T{0:%T%z} [{1:>7}] {2}: {3}", entry.time, LogLevelToString(entry.level), entry.category.GetName(), entry.message)
            .Unwrap();
    }
}