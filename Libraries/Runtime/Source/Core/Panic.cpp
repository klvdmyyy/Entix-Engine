#include "Core/Panic.h"

#include "Core/Debug/Log.h"

#include <print>
#include <filesystem>

void Panic(StringView message, std::source_location location)
{
    // EX_LOG_LOC(location, R"(ENGINE PANIC
    //     Message: {}
    //     Function Name: {}
    //     Source Location: {}({}:{}))",
    // message,
    // std::filesystem::path(location.file_name()).filename().string(),
    // location.function_name(), location.line(), location.column());

    EX_DEBUGBREAK();
    std::exit(1);
}