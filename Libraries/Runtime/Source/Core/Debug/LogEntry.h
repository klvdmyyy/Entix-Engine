// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/String.h"
#include "Core/Memory.h"

#include "Core/IO/Base.h"

#include "Core/Debug/LogLevel.h"

#include <source_location>

#define GLOBAL_LOG_CATEGORY_PREFIX(NAME) g_logCategory_##NAME

#define DEFINE_LOG_CATEGORY(NAME, MIN_LEVEL) \
    static constexpr LogCategory GLOBAL_LOG_CATEGORY_PREFIX(NAME) = LogCategory{#NAME, MIN_LEVEL}

struct LogEntry;

class LogFormatter;
class LogCategory;

class ERUNTIME_API LogFormatter {
public:
    virtual ~LogFormatter() = default;

    virtual String Format(LogEntry entry) const noexcept = 0;
};

class ERUNTIME_API LogCategory {
public:
    constexpr LogCategory(const String& name, LogLevel minLevel)
        : m_name(name), m_minLevel(minLevel)
    {
    }

    constexpr const String& GetName() const noexcept { return m_name; }
    constexpr LogLevel GetMinLevel() const noexcept { return m_minLevel; }
    constexpr bool IsEnabled(LogLevel level) const noexcept { return (uint8_t)level >= (uint8_t)m_minLevel; }

    void operator=(const LogCategory& other)
    {
       m_name = other.m_name;
       m_minLevel = other.m_minLevel;
    }

private:
    String m_name;
    LogLevel m_minLevel;
};

class ERUNTIME_API LogEntry : public IO::Packet
{
public:
    static constexpr uint16_t MAGIC_NUMBER = 0x474C;

    LogEntry() = default;

    LogEntry(LogLevel level_, LogCategory category_, const String& message_)
        : level(level_), category(category_), message(message_)
    {
    }

    LogLevel level = LogLevel::Trace;
    LogCategory category = LogCategory{"Unknown", LogLevel::Trace};
    String message{};

    uint16_t GetMagic() const noexcept override { return MAGIC_NUMBER; }

    void Pack(IO::Writer& writer) const override;
    bool Unpack(IO::Reader& reader) override;
};