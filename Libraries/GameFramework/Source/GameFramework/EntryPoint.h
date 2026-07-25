// -*- mode: c++; -*-
/* Entry Point */

#pragma once

#include "Core/Base.h"

#include "GameFramework/Application.h"

#include "Core/Debug/Log.h"
#include "Core/IO/FileWriter.h"

#include <chrono>
#include <format>

extern Application* CreateApplication();

static const LogCategory LogTemp = LogCategory("Temp", LogLevel::Trace);
    
static int GameMain(int argc, char** argv)
{
    auto fileWriter = CreateScope<IO::FileWriter>(std::filesystem::current_path() / std::filesystem::path("log.txt"));
    auto logWriter = CreateScope<HumanReadableLogWriter>(std::move(fileWriter));
    
    Logger::Instance().AddWriter(std::move(logWriter), {
        LogLevel::Trace,
        LogLevel::Info,
        LogLevel::Warning,
        LogLevel::Error,
        LogLevel::Critical
    });

    Logger::Instance().WriteMessage(LogTemp, LogLevel::Info, "Hello!");

    auto app = CreateApplication();
        
    app->Run(argc, argv);
        
    delete app;
        
    return 0;
}

#if defined(PLATFORM_LINUX) || defined(PLATFORM_WINDOWS)
int main(int argc, char** argv)
{
    return ::GameMain(argc, argv);
}
#elif defined(PLATFORM_WINDOWS)
#include <Windows.h>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    return ::GameMain(__argc, __argv);
}
#else
#error "No supported entry point for target platform."
#endif
