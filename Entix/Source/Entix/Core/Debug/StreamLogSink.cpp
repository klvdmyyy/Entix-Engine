#include "Entix/Core/Debug/StreamLogSink.h"

#include "Entix/Core/IO/TextStream.h"

#include <thread>

namespace Entix
{
    void StreamLogSink::WriteLogEntry(LogEntry entry)
    {
        std::scoped_lock<std::mutex> lock(m_sync);
        
        IO::TextStream::CreateNonOwned(*m_stream)
            .WriteLineFmt("{0:%F}T{0:%T%z} thread={1:<6} [{2:>7}] {3}: {4}", entry.time, std::this_thread::get_id(), LogLevelToString(entry.level), entry.category.GetName(), entry.message)
            .Unwrap();
    }
}