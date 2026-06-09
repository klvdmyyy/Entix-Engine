#pragma once

#ifdef _WIN32
#ifdef ERUNTIME_BUILD_DLL
#define ERUNTIME_EXPORT __declspec(dllexport)
#else
#define ERUNTIME_EXPORT __declspec(dllimport)
#endif
#else
#define ERUNTIME_EXPORT
#endif