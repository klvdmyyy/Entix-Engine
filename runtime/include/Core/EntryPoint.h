// -*- mode: c++; -*-
/* Entry Point */

#pragma once

#include "Core/Base.h"

#include "Core/Application.h"

namespace ERUNTIME_NAMESPACE {
    extern Application* CreateApplication();
    
    int Main(int argc, char** argv)
    {
        auto app = CreateApplication();
        app->Run(argc, argv);
        delete app;
        return 0;
    }
}

#if defined(PLATFORM_LINUX)
int main(int argc, char** argv)
{
    return ::ERUNTIME_NAMESPACE::Main(argc, argv);
}
#elif defined(PLATFORM_WINDOW)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    return ::ERUNTIME_NAMESPACE::Main(__argc, __argv);
}
#else
#error "No supported entry point for target platform."
#endif

// TODO...