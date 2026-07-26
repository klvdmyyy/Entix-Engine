#include "Entix/Core/Debug/Logger.h"

#include <print>

namespace Entix
{
    Logger& Logger::Instance()
    {
        static Logger s_instance;
        return s_instance;
    }

    void Logger::AddSink(Scope<LogSink> sink, std::initializer_list<LogLevel> enabledLevels)
    {
        LevelBitset levels;
        std::for_each(enabledLevels.begin(), enabledLevels.end(), [&](LogLevel level) { levels.set(static_cast<Uint8>(level)); });

        m_sinks.push_back({std::move(sink), levels});
    }

    void Logger::LogMessage(
        LogCategory category,
        LogLevel level,
        StringView message
    )
    {
        for(auto& [sink, enabledLevels] : m_sinks)
        {
            if(!enabledLevels[static_cast<Uint8>(level)] || !category.IsEnabled(level)) continue;

            LogEntry entry {
                .time = LogEntry::Clock::now(),
                .category = category,
                .level = level,
                .message = message
            };

            sink->WriteLogEntry(entry);
        }
    }
}