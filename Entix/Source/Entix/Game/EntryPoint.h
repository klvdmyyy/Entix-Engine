#pragma once

#include "Entix/Core/PlatformDetection.h"

#include "Entix/Core/Debug/Logger.h"

#include "Entix/Core/Debug/StdoutLogSink.h"
#include "Entix/Core/Debug/StreamLogSink.h"

#include "Entix/Core/IO/FileStream.h"

#include "Entix/Core/Globals.h"

#include "Entix/Game/Application.h"

#include <chrono>
#include <format>

namespace Entix
{
    extern Application* CreateApplication();

    static int Main(int argc, char** argv)
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

#ifndef ENTIX_PLATFORM_WINDOWS
        Logger::Instance().AddSink(CreateScope<StdoutLogSink>(), {
            LogLevel::Debug,
            LogLevel::Info,
            LogLevel::Warning,
            LogLevel::Error,
            LogLevel::Fatal
        });
#endif

        EX_LOG(LogTemp, Info, "Running game's `CreateApplication()`...");

        auto app = CreateApplication();

        auto res = app->Run();

        delete app;

        return res.IsSuccess() ? 0 : 1;
    }
}

#if defined(ENTIX_PLATFORM_WINDOWS)
#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    return ::Entix::Main(__argc, __argv);
}
#else
int main(int argc, char** argv)
{
    return ::Entix::Main(argc, argv);
}
#endif