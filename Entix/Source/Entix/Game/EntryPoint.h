#pragma once

#include "Entix/Core/Debug/Logger.h"

#include "Entix/Core/Debug/StdoutLogSink.h"
#include "Entix/Core/Debug/StreamLogSink.h"

#include "Entix/Core/IO/FileStream.h"

#include "Entix/Core/Globals.h"
#include "Entix/Core/Memory.h"

#include "Entix/Game/Application.h"

#include <CLI/CLI.hpp>

#include <chrono>
#include <format>

namespace Entix
{
    extern ApplicationDesc CreateApplication(int argc, char** argv);

    static int Main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
    {
        auto file = CreateScope<IO::FileStream>(std::format("{0:%F}T{0:%H-%M%Z}.log", std::chrono::utc_clock::now()), IO::StreamMode::Write);
        Logger::Instance().AddSink(CreateScope<StreamLogSink>(std::move(file)), {
            LogLevel::Trace,
            LogLevel::Debug,
            LogLevel::Info,
            LogLevel::Warning,
            LogLevel::Error,
            LogLevel::Fatal
        });

        // Windows applications are compiled with WinMain which
        // doesn't use terminal.
        //
        // We just don't need to write logs to stdout.
#ifndef ENTIX_PLATFORM_WINDOWS
        Logger::Instance().AddSink(CreateScope<StdoutLogSink>(), {
            LogLevel::Trace,
            LogLevel::Debug,
            LogLevel::Info,
            LogLevel::Warning,
            LogLevel::Error,
            LogLevel::Fatal
        });
#endif

        EX_LOG(LogTemp, Info, "Running game's `CreateApplication()`...");

        try
        {
            auto desc = CreateApplication(argc, argv);

            // Parsing command line arguments and ovverride some parameters of ApplicationDesc
            {
                CLI::App cliApp{desc.description};
                argv = cliApp.ensure_utf8(argv);

                cliApp.add_option("--threads", desc.threads, "Thread pool worker count.");
                cliApp.add_option("--hot-reload", desc.enableHotReload, "Enable ResourceManager hot-reload.");

                String rhiBackendStr = "vulkan";
                cliApp.add_option("--rhi-backend", rhiBackendStr, "Rendering Hardware Interface backend API.");

                CLI11_PARSE(cliApp, argc, argv);

                // Parse RHI::BackendApi from provided string option
                desc.rhiFactoryDesc.backendApi = FromString<RHI::BackendApi>(rhiBackendStr);
            }

            Application app(desc);

            app.Run();
            
            return 0;
        }
        catch(const std::exception& e)
        {
            EX_LOG(LogTemp, Info, "Exception is catched. What: {}", e.what());
            return 1;
        }
    }
}

#if defined(ENTIX_PLATFORM_WINDOWS)
#include <Windows.h>

int APIENTRY WinMain(
    [[maybe_unused]] HINSTANCE hInstance,
    [[maybe_unused]] HINSTANCE hPrevInstance,
    [[maybe_unused]] LPSTR lpCmdLine,
    [[maybe_unused]] int nShowCmd
)
{
    return ::Entix::Main(__argc, __argv);
}
#else
// NOLINTNEXTLINE
int main(int argc, char** argv)
{
    return ::Entix::Main(argc, argv);
}
#endif
