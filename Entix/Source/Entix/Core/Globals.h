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
    EX_DEFINE_LOG_CATEGORY(LogTemp, Info);
    EX_DEFINE_LOG_CATEGORY(LogIO, Trace);
}

/**
 * @}
 */