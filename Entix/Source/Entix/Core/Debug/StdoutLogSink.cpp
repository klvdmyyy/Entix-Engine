#include "Entix/Core/Debug/StdoutLogSink.h"

#include <print>

namespace Entix
{
    void StdoutLogSink::WriteLogEntry(LogEntry entry)
    {
        if(entry.level == LogLevel::Error || entry.level == LogLevel::Fatal)
            std::println(stderr, "{0:%F}T{0:%R%z} [{1}] {2}: {3}", entry.time, entry.level, entry.category.GetName(), entry.message);
        else
            std::println(stdout, "{0:%F}T{0:%R%z} [{1}] {2}: {3}", entry.time, entry.level, entry.category.GetName(), entry.message);
    }
}