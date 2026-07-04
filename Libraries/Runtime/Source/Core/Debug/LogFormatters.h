// -*- mode: c++; -*-
#pragma once

#include "Core/Debug/LogEntry.h"

#include <format>

class DefaultFormatter : public LogFormatter {
public:
    String Format(LogEntry entry) const noexcept final
    {
        return std::format("{}({}:{}) `{}` [{}:{}] {}",
                           entry.location.file_name(),
                           entry.location.line(),
                           entry.location.column(),
                           entry.location.function_name(),
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