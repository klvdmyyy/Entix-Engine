#pragma once

#include "Core/Base.h"

#include <memory>

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename... Args>
FORCE_INLINE
constexpr Scope<T> CreateScope(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
FORCE_INLINE
constexpr Ref<T> CreateRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}