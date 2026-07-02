#pragma once

#include "Core/Debug/Log.h"

#include "Core/Types.h"

#include "Core/IO/Writer.h"

#include <print>
#include <deque>

class BufferLogSink : public LogSink, public IO::Writer
{
public:
    static constexpr Uint64 MAX_ENTRY_COUNT = 100;

    static BufferLogSink& Instance();

    void Write(StringView str) final
    {
        if(m_entries.size() > MAX_ENTRY_COUNT)
            m_entries.pop_front();
            
        m_entries.push_back(String(str));
    }

    inline void Write(const LogEntry& entry) final
    {
        if(m_entries.size() > MAX_ENTRY_COUNT)
            m_entries.pop_front();
            
        m_entries.push_back(FormatLogEntry(entry));
    }

    const std::deque<String>& GetEntries() const noexcept
    {
        return m_entries;
    }

private:
    BufferLogSink() = default;
        
    std::deque<String> m_entries{};
};

class StdoutLogSink : public LogSink
{
public:
    inline void Write(const LogEntry& entry) final
    {
        if(entry.level == LogLevel::Error || entry.level == LogLevel::Critical)
            std::println(stderr, "{}", FormatLogEntry(entry));
        else
            std::println("{}", FormatLogEntry(entry));
    }
};