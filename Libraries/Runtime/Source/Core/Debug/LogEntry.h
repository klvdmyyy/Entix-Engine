// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/String.h"
#include "Core/Memory.h"

#include "Core/Debug/LogLevel.h"

#include <source_location>

struct LogEntry;

class LogFormatter;
class LogCategory;

class LogFormatter {
public:
    virtual ~LogFormatter() = default;

    virtual String Format(LogEntry entry) const noexcept = 0;
};

class LogCategory {
public:
    LogCategory(const char* name, const LogFormatter* formatter)
        : k_name(name), k_formatter(formatter)
    {
    }

    const char* GetName() const { return k_name; }
    const LogFormatter* GetFormatter() const { return k_formatter; }

    static const LogCategory Core;
    static const LogCategory IO;
    static const LogCategory WSI;
    static const LogCategory Renderer;
    static const LogCategory Driver;
    static const LogCategory Resource;
    static const LogCategory Console;

private:
    const char* k_name;
    const LogFormatter* k_formatter;
};

struct LogEntry
{
    LogLevel level;
    LogCategory category;
    StringView message;
    std::source_location location;
};