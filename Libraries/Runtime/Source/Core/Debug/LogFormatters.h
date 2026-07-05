// -*- mode: c++; -*-
#pragma once

#include "Core/Debug/LogEntry.h"

#include <format>
#include <filesystem>
#include <print>

class DefaultFormatter : public LogFormatter {
public:
    String Format(LogEntry entry) const noexcept final
    {
        auto filepath = std::filesystem::path(entry.location.file_name());

        return std::format("{}({}:{}) [{}:{}] {}",
                           filepath.filename(),
                           entry.location.line(),
                           entry.location.column(),
                           // entry.location.function_name(),
                           entry.category.GetName(),
                           entry.level,
                           entry.message);
    }
};

class ConsoleFormatter : public LogFormatter {
public:
    String Format(LogEntry entry) const noexcept final
    {
        return std::format("[{}:{}] {}",
                           entry.category.GetName(),
                           entry.level,
                           entry.message);
    }
};