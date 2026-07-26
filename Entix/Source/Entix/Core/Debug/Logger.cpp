#include "Entix/Core/Debug/Logger.h"

#include <print>

namespace Entix
{
    Logger& Logger::Instance()
    {
        static Logger s_instance;
        return s_instance;
    }

    void Logger::LogMessage(
        LogCategory category,
        LogLevel level,
        StringView message
    )
    {
        std::println("[{}] {}: {}", level, category.GetName(), message);
    }
}