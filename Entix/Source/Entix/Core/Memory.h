/**
 * @file
 * @brief Memory things
 * 
 * @ingroup Core
 */

#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Types.h"

#include <memory>

/**
 * @brief Allocation overloading
 * 
 * @details
 * Used only for tracing/profiling in Debug mode.
 * 
 * @ingroup Core
 */
extern void* operator new(size_t); // NOLINT

/**
 * @brief Allocation overloading
 * 
 * @details
 * Used only for tracing/profiling in Debug mode.
 * 
 * @ingroup Core
 */
extern void operator delete(void*);

namespace Entix
{
    /**
     * @brief Scoped pointer
     * 
     * @typedef std::unique_ptr
     * 
     * @ingroup Core
     */
    template<typename T>
    using Scope = std::unique_ptr<T>;

    /**
     * @brief Creates scoped pointer
     * 
     * @details
     * Just an alias for std::make_unique
     * 
     * @ingroup Core
     */
    template<typename T, typename... Args>
    EX_FORCE_INLINE
    inline Scope<T> CreateScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    /**
     * @brief Reference Counter
     * 
     * @typedef std::shared_ptr
     * 
     * @ingroup Core
     */
    template<typename T>
    using Ref = std::shared_ptr<T>;

    /**
     * @brief Creates reference counter
     * 
     * @details
     * Just an alias for std::make_shared
     * 
     * @ingroup Core
     */
    template<typename T, typename... Args>
    EX_FORCE_INLINE
    inline Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}