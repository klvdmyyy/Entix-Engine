// -*- mode: c++; -*-
#pragma once

#include "Core/Debug/LogEntry.h"

#include <format>
#include <filesystem>
#include <print>

class ERUNTIME_API DefaultFormatter : public LogFormatter {
public:
    static DefaultFormatter* Instance();

    String Format(LogEntry entry) const noexcept final
    {
        auto filepath = std::filesystem::path(entry.location.file_name());

        return std::format("{}({}:{}) [{}:{}] {}",
                           filepath.filename().string(),
                           entry.location.line(),
                           entry.location.column(),
                           // entry.location.function_name(),
                           entry.category.GetName(),
                           entry.level,
                           entry.message);
    }
};

class ERUNTIME_API ConsoleFormatter : public LogFormatter {
public:
    static ConsoleFormatter* Instance();

    String Format(LogEntry entry) const noexcept final
    {
        return std::format("[{}:{}] {}",
                           entry.category.GetName(),
                           entry.level,
                           entry.message);
    }
};