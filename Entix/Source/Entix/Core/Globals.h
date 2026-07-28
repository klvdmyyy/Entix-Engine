/**
 * @file
 * @brief Collection of global variables for the Engine
 * 
 * @details
 * Mainly it contains constexpr values. Global variables only allowed
 * when no other alternatives.
 * 
 * @ingroup Core
 * @{
 */

#pragma once

#include "Entix/Core/Debug/LogCategory.h"

namespace Entix
{
    // Panic-only category
    EX_DEFINE_LOG_CATEGORY(LogPanic, Fatal);

    EX_DEFINE_LOG_CATEGORY(LogTemp, Info);
    EX_DEFINE_LOG_CATEGORY(LogIO, Trace);

    EX_DEFINE_LOG_CATEGORY(LogRHI, Trace);

    EX_DEFINE_LOG_CATEGORY(Resources, Trace);

    EX_DEFINE_LOG_CATEGORY(ShaderCompilation, Trace);

    EX_DEFINE_LOG_CATEGORY(VulkanGeneral, Debug);
    EX_DEFINE_LOG_CATEGORY(VulkanPerformance, Warning);
    EX_DEFINE_LOG_CATEGORY(VulkanValidation, Trace);
}

/**
 * @}
 */