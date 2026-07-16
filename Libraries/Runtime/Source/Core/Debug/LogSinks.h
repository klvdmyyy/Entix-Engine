#pragma once

#include "Core/Debug/Log.h"

#include "Core/Types.h"

#include "Core/IO/Base.h"

#include <print>
#include <deque>

class ERUNTIME_API BufferLogSink : public LogSink, public IO::Writer
{
public:
    static constexpr Uint64 MAX_ENTRY_COUNT = 100;

    static BufferLogSink& Instance();

    size_t Write(const void* buffer, size_t size) final
    {
        if(m_entries.size() > MAX_ENTRY_COUNT)
            m_entries.pop_front();
        
        String str;
        str.resize(size);
        str = static_cast<const char*>(buffer);

        m_entries.push_back(String(str));

        return size;
    }

    void WriteLogEntry(const LogEntry& entry) final
    {
        if(m_entries.size() > MAX_ENTRY_COUNT)
            m_entries.pop_front();
            
        m_entries.push_back(entry.category.GetFormatter()->Format(entry));
    }

    const std::deque<String>& GetEntries() const noexcept
    {
        return m_entries;
    }

private:
    BufferLogSink() = default;
        
    std::deque<String> m_entries{};
};

class ERUNTIME_API StdoutLogSink : public LogSink
{
public:
    inline void WriteLogEntry(const LogEntry& entry) final
    {
        if(entry.level == LogLevel::Error || entry.level == LogLevel::Critical)
            std::println(stderr, "{}", entry.category.GetFormatter()->Format(entry));
        else
            std::println("{}", entry.category.GetFormatter()->Format(entry));
    }
};