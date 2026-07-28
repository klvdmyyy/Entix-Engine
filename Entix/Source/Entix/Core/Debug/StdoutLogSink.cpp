#include "Entix/Core/Debug/StdoutLogSink.h"

#include <print>
#include <thread>

namespace Entix
{
    void StdoutLogSink::WriteLogEntry(LogEntry entry)
    {
        if(entry.level == LogLevel::Error || entry.level == LogLevel::Fatal)
            std::println(stderr, "{0:%F}T{0:%T%z} thread={1} [{2:>7}] {3}: {4}", entry.time, std::this_thread::get_id(), LogLevelToString(entry.level), entry.category.GetName(), entry.message);
        else
            std::println(stdout, "{0:%F}T{0:%T%z} thread={1} [{2:>7}] {3}: {4}", entry.time, std::this_thread::get_id(), LogLevelToString(entry.level), entry.category.GetName(), entry.message);
    }
}