#ifndef ENTIX_BUILD_DLL
#pragma once

#include "Entix/Core/PlatformDetection.h"

namespace Entix
{
    extern int Main(int argc, char** argv);
}

#if defined(ENTIX_PLATFORM_WINDOWS)
#include <Windows.h>
//! [entry_windows]
int APIENTRY WinMain(
    [[maybe_unused]] HINSTANCE hInstance,
    [[maybe_unused]] HINSTANCE hPrevInstance,
    [[maybe_unused]] LPSTR lpCmdLine,
    [[maybe_unused]] int nShowCmd
)
{
    return ::Entix::Main(__argc, __argv);
}
//! [entry_windows]
#else
//! [entry_linux]
int main(int argc, char** argv) // NOLINT
{
    return ::Entix::Main(argc, argv);
}
//! [entry_linux]
#endif

#endif
