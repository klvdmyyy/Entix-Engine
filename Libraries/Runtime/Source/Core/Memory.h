#pragma once

#include "Core/Base.h"

#include <memory>

void* operator new(std::size_t count);
void operator delete(void* ptr) noexcept;

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