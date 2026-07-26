#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/String.h"

#include <source_location>

namespace Entix
{
    ENTIX_API void Panic [[noreturn]] (std::source_location location = std::source_location::current());
    ENTIX_API void Panic [[noreturn]] (StringView message, std::source_location location = std::source_location::current());
}