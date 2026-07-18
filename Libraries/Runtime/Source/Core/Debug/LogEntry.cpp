#include "Core/Debug/LogEntry.h"

#include "Core/Debug/LogFormatters.h"

static DefaultFormatter* g_defaultFormatter = DefaultFormatter::Instance();
static ConsoleFormatter* g_consoleFormatter = ConsoleFormatter::Instance();

const LogCategory LogCategory::Core = LogCategory("Core", g_defaultFormatter);
const LogCategory LogCategory::IO = LogCategory("I/O", g_defaultFormatter);
const LogCategory LogCategory::WSI = LogCategory("WSI", g_defaultFormatter);
const LogCategory LogCategory::Renderer = LogCategory("Renderer", g_defaultFormatter);
const LogCategory LogCategory::Driver = LogCategory("Driver", g_defaultFormatter);
const LogCategory LogCategory::Resource = LogCategory("Resource", g_defaultFormatter);
const LogCategory LogCategory::Console = LogCategory("Console", g_consoleFormatter);