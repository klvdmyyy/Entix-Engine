/**
 * @file
 * @brief Collection of compiler detection macro-definitions
 * 
 * @ingroup Core
 */

#pragma once

#if defined(__clang__)
    #define ENTIX_COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
    #define ENTIX_COMPILER_GCC
#elif defined(_MSC_VER)
    #define ENTIX_COMPILER_MSVC
#else
    // #warning "Unknown or unsupported compiler. It can cause errors!"
#endif