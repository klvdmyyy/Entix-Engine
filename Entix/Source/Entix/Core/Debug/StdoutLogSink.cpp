#include "Entix/Core/Debug/StdoutLogSink.h"

#include <print>

namespace Entix
{
    void StdoutLogSink::WriteLogEntry(LogEntry entry)
    {
        if(entry.level == LogLevel::Error || entry.level == LogLevel::Fatal)
            std::println(stderr, "[{}] {}: {}", entry.level, entry.category.GetName(), entry.message);
        else
            std::println(stdout, "[{}] {}: {}", entry.level, entry.category.GetName(), entry.message);
    }
}