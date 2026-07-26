#pragma once

#include "LogEntry.h"

#include <format>

#define EX_LOG(CATEGORY, LEVEL, FMT, ...) \
    ::Entix::Logger::Instance().LogMessage(g_logCategory_##CATEGORY, ::Entix::LogLevel::LEVEL, std::format(FMT,##__VA_ARGS__))

namespace Entix
{
    class ENTIX_API Logger
    {
    public:
        static Logger& Instance();

        void LogMessage(
            LogCategory category,
            LogLevel level,
            StringView message
        );
    };
}