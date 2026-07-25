#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Types.h"

#include <memory>

extern void* operator new(size_t);
extern void operator delete(void*);

namespace Entix
{
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename... Args>
    EX_FORCE_INLINE
    inline Scope<T> CreateScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    EX_FORCE_INLINE
    inline Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}