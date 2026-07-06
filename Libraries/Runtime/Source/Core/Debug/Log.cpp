#include "Core/Debug/Log.h"

#include "Core/Assert.h"

Logger& Logger::Instance()
{
    static Logger s_logger;
    return s_logger;
}

void Logger::AddSink(Ref<LogSink> sink) {
    m_sinks.push_back(std::move(sink));
}

void Logger::Log(LogLevel level, LogCategory category, StringView message,
                 std::source_location location) {
    for (auto& sink : m_sinks) {
        sink->WriteLogEntry(LogEntry{.level = level,
                                     .category = category,
                                     .message = message,
                                     .location = location});
    }
}