// -*- mode: c++; -*-
/* Entry Point */

#pragma once

#include "Core/Base.h"

#include "GameFramework/Application.h"

#include "Core/Debug/Log.h"
#include "Core/Debug/LogSinks.h"

extern Application* CreateApplication();
    
static int GameMain(int argc, char** argv)
{
    Logger::Instance().AddSink(CreateRef<StdoutLogSink>());
    Logger::Instance().AddSink(Ref<LogSink>(&BufferLogSink::Instance(), [](void*){}));
        
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
