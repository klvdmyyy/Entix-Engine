#pragma once

#ifdef _WIN32
#include <Windows.h>
#define GameMain() WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int mShowCmd)
#define CollectArgs() int argc = __argc; char** argv = __argv
#else
#define GameMain() main(int argc, char** argv)
#define CollectArgs() do {} while(0)
#endif
