#include "Entix/Core/Panic.h"

#include "Entix/Core/Debug/Logger.h"
#include "Entix/Core/Globals.h"

#include <filesystem>
#include <exception>

#if defined(ENTIX_PLATFORM_LINUX)
#include <sys/types.h>
#include <sys/ptrace.h>
#elif defined(ENTIX_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

namespace Entix
{
    namespace Utils
    {
#if defined(ENTIX_BUILD_DEBUG)
        static bool IsRunningWithDebugger();

#if defined(ENTIX_PLATFORM_LINUX) 
        static int underDebugger = 0;
        // This works on both linux and MacOSX (and any BSD kernel).
        bool IsRunningWithDebugger()
        {
            static bool isCheckedAlready = false;
            if (!isCheckedAlready)
            {
                if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0)
                    underDebugger = 1;
                else ptrace(PTRACE_DETACH, 0, 1, 0);

                isCheckedAlready = true;
            }
            return underDebugger == 1;
        }
#elif defined(ENTIX_PLATFORM_WINDOWS)
        bool IsRunningWithDebugger()
        {
            return IsDebuggerPresent() == TRUE;
        }
#else
        bool IsRunningWithDebugger()
        {
            return false;
        }
#endif

#endif
    }

    void Panic(std::source_location location)
    {
        Panic("No Message", location);
    }

    void Panic(StringView message, std::source_location location)
    {
        EX_LOG(
            LogPanic, Fatal, "{}({}:{}): Panic: {}",
            std::filesystem::path(location.file_name()).filename().string(),
            location.line(), location.column(),
            message
        );

#if defined(ENTIX_PLATFORM_WINDOWS) && !defined(ENTIX_BUILD_DEBUG)
        MessageBoxA(
            nullptr,
            std::format(
                "Panic caused at {}({}:{}):\n{}",
                std::filesystem::path(location.file_name()).filename().string(),
                location.line(), location.column(), message
            ).c_str(),
            "Engine Panic",
            MB_OK
        );
#endif

#ifdef ENTIX_BUILD_DEBUG
        if(Utils::IsRunningWithDebugger())
        {
            EX_DEBUGBREAK();
        }
#endif
        
        throw std::runtime_error("PANIC");
    }
}