#pragma once

#if defined(__clang__)
    #define COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
    #define COMPILER_GCC
#elif defined(_MSC_VER)
    #define COMPILER_MSVC
#else
    // #warning "Unknown or unsupported compiler. It can cause errors!"
#endif