#pragma once

#include <functional>

namespace Entix
{
    // from: https://stackoverflow.com/a/57595105
    //
    // Sometimes we need to hash data structure by two or more values.
    // 
    // This simple function make it as easy as writing "Hello, World" app.
    template <typename T, typename... Rest>
    void HashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
        seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        (HashCombine(seed, rest), ...);
    };
}