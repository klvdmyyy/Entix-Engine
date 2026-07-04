#pragma once

#include "Core/String.h"
#include "Core/Base.h"
#include "Core/Memory.h"

#include "Core/Debug/LogEntry.h"

#include <algorithm>
#include <mutex>
#include <format>
#include <vector>
#include <source_location>

#define EX_LOG(Level, Category, Fmt, ...) \
    ::Logger::Instance().Log(LogLevel::Level, Category, std::format(Fmt,##__VA_ARGS__))

class LogSink {
public:
    LogSink() = default;
    virtual ~LogSink() = default;

    virtual void WriteLogEntry(const LogEntry &item) = 0;
};

class Logger {
public:
    static Logger &Instance();
      
    void AddSink(Ref<LogSink> sink);

    void Log(LogLevel level, LogCategory category, StringView message,
             std::source_location location = std::source_location::current());

private:
    std::vector<Ref<LogSink>> m_sinks;
    std::mutex m_sync;
};


namespace Debug {
#define LINSTANCE() Logger::Instance()

    FORCE_INLINE
    inline constexpr void Trace(LogCategory category, StringView message,
                                std::source_location location = std::source_location::current())
    {
        LINSTANCE().Log(LogLevel::Trace, category, message, location);
    }

    FORCE_INLINE
    inline constexpr void Info(LogCategory category, StringView message,
                               std::source_location location = std::source_location::current())
    {
        LINSTANCE().Log(LogLevel::Info, category, message, location);
    }

    FORCE_INLINE
    inline constexpr void Warn(LogCategory category, StringView message,
                               std::source_location location = std::source_location::current())
    {
        LINSTANCE().Log(LogLevel::Warning, category, message, location);
    }

    FORCE_INLINE
    inline constexpr void Error(LogCategory category, StringView message,
                                std::source_location location = std::source_location::current())
    {
        LINSTANCE().Log(LogLevel::Error, category, message, location);
    }
    
    FORCE_INLINE
    inline constexpr void Critical(LogCategory category, StringView message,
                                   std::source_location location = std::source_location::current())
    {
        LINSTANCE().Log(LogLevel::Critical, category, message, location);
    }
        
#undef LINSTANCE
}

