#pragma once

#include "Core/String.h"
#include "Core/Base.h"
#include "Core/Memory.h"

#include <algorithm>
#include <sstream>
#include <type_traits>
#include <mutex>
#include <format>
#include <vector>
#include <chrono>
#include <optional>

#define EX_LOG(LEVEL, CATEGORY, FMT, ...)                               \
    ::ERUNTIME_NAMESPACE::Logger::Instance().Log(::ERUNTIME_NAMESPACE::LogLevel::LEVEL, CATEGORY, \
                                                 std::format(FMT, ##__VA_ARGS__), __FILE__, __LINE__, __FUNCTION__)

namespace ERUNTIME_NAMESPACE
{
    namespace LogCategory
    {
        constexpr auto Core = "Core";
        constexpr auto IO = "I/O";
        constexpr auto WSI = "WSI";
        constexpr auto Renderer = "Renderer";
        constexpr auto Driver = "Driver";
        
        constexpr auto Console = "Console";
    }

    enum class LogLevel
        {
            Trace = 0,
            Info,
            Warning,
            Error,
            Critical,
        };

    [[nodiscard]]
    const StringView LogLevelToString(const LogLevel& level);

    struct LogEntry
    {
        LogLevel level;
        const char *category;
        StringView message;
        std::optional<const char *> sourceFile;
        std::optional<int> line;
        std::optional<const char *> functionSignature;
    };

    String FormatLogEntry(const LogEntry& entry);

    class ERUNTIME_API LogSink
        {
        public:
        LogSink() = default;
        virtual ~LogSink() = default;

        virtual void Write(const LogEntry &item) = 0;
        };

    class ERUNTIME_API Logger
        {
        public:
        static Logger &Instance();
      
        void AddSink(Ref<LogSink> sink);

        void Log(LogLevel level, const char *category, StringView message,
                 std::optional<const char *> sourceFile = std::nullopt,
                 std::optional<int> line = std::nullopt,
                 std::optional<const char *> functionSignature = std::nullopt);

        private:
        std::vector<Ref<LogSink>> m_sinks;
        std::mutex m_sync;
        };


    namespace Debug {
#define LINSTANCE() Logger::Instance()

        template<typename... Args>
        void Trace(const char* category, std::format_string<Args...> fmt, Args&&... args)
        {
            LINSTANCE().Log(LogLevel::Trace, category, std::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        void Info(const char* category, std::format_string<Args...> fmt, Args&&... args)
        {
            LINSTANCE().Log(LogLevel::Info, category, std::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        void Warn(const char* category, std::format_string<Args...> fmt, Args&&... args)
        {
            LINSTANCE().Log(LogLevel::Warning, category, std::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        void Error(const char* category, std::format_string<Args...> fmt, Args&&... args)
        {
            LINSTANCE().Log(LogLevel::Error, category, std::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        void Critical(const char* category, std::format_string<Args...> fmt, Args&&... args)
        {
            LINSTANCE().Log(LogLevel::Critical, category, std::format(fmt, std::forward<Args>(args)...));
        }
        
#undef LINSTANCE
    }
}

template<>
struct std::formatter<ERUNTIME_NAMESPACE::LogLevel, char>
{
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
        auto it = ctx.begin();
        return it;
    }

    template<typename FmtContext>
    FmtContext::iterator format(ERUNTIME_NAMESPACE::LogLevel level,
                                FmtContext &ctx) const
    {
        std::ostringstream out;
        out << ERUNTIME_NAMESPACE::LogLevelToString(level);

        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};
