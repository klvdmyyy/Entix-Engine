#pragma once

#include "Core/String.h"
#include "Core/Base.h"
#include "Core/Memory.h"

#include "Core/IO/Base.h"
#include "Core/IO/Decorators.h"
#include "Core/IO/MemoryReader.h"

#include "Core/Debug/LogEntry.h"

#include <algorithm>
#include <mutex>
#include <format>
#include <vector>
#include <source_location>
#include <bitset>
#include <unordered_map>

// #define EX_LOG(Level, Category, Fmt, ...) \
//     ::Logger::Instance().Log(LogLevel::Level, GLOBAL_LOG_CATEGORY_PREFIX(Category), std::format(Fmt,##__VA_ARGS__))

// #define EX_LOG_LOC(Location, Level, Category, Fmt, ...) \
//     ::Logger::Instance().Log(LogLevel::Level, GLOBAL_LOG_CATEGORY_PREFIX(Category), std::format(Fmt,##__VA_ARGS__), Location)

#define EX_LOG(...)

class HumanReadableLogWriter : public IO::WriterDecorator {
    DEFINE_WRITER_DECORATOR(HumanReadableLogWriter);

public:
    size_t Write(const void* data, size_t size) override;
};

class ERUNTIME_API Logger {
public:
    using LevelBitset = std::bitset<(uint8_t)LogLevel::Count>;

    Logger() = default;

    // Unable to copy
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Unable to move
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    static Logger& Instance();

    void AddWriter(Scope<IO::Writer> writer, std::initializer_list<LogLevel> enabledLevels);

    void WriteMessage(
        LogCategory category,
        LogLevel level,
        StringView message,
        std::source_location location = std::source_location::current()
    );

private:
    std::vector<std::pair<Scope<IO::Writer>, LevelBitset>> m_writers;
};
