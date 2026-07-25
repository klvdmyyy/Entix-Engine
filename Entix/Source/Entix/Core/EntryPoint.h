#pragma once

#include "Entix/Core/PlatformDetection.h"

namespace Entix
{
    extern int Main(int argc, char** argv);
}

#if defined(ENTIX_PLATFORM_WINDOWS_)
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