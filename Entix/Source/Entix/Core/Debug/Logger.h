#pragma once

#include "Entix/Core/Memory.h"

#include "LogEntry.h"
#include "LogSink.h"

#include <bitset>
#include <vector>
#include <format>

#define EX_LOG(CATEGORY, LEVEL, FMT, ...) \
    ::Entix::Logger::Instance().LogMessage(g_logCategory_##CATEGORY, ::Entix::LogLevel::LEVEL, std::format(FMT,##__VA_ARGS__))

namespace Entix
{
    class Logger
    {
    public:
        using LevelBitset = std::bitset<static_cast<Uint8>(LogLevel::Count)>;

        // Unable to copy
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        // Unable to move
        Logger(Logger&&) = delete;
        Logger& operator=(Logger&&) = delete;

        ENTIX_API static Logger& Instance();

        ENTIX_API void AddSink(Scope<LogSink> sink, std::initializer_list<LogLevel> enabledLevels);

        ENTIX_API void LogMessage(
            LogCategory category,
            LogLevel level,
            StringView message
        );

    private:
        Logger() = default;
        
        std::vector<std::pair<Scope<LogSink>, LevelBitset>> m_sinks;
    };
}