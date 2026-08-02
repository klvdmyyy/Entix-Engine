#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Types.h"
#include "Entix/Core/String.h"

#include "Entix/Core/Debug/LogLevel.h"

#define EX_GLOBAL_LOG_CATEGORY_NAME(NAME) g_logCategory_##NAME

#define EX_DEFINE_LOG_CATEGORY(NAME, LEVEL) \
    static constexpr auto EX_GLOBAL_LOG_CATEGORY_NAME(NAME) = ::Entix::LogCategory(#NAME, ::Entix::LogLevel::LEVEL)

namespace Entix
{
    class LogCategory
    {
    public:
        ENTIX_API constexpr LogCategory(StringView name, LogLevel minLevel)
            : k_name(name), k_minLevel(minLevel)
        {
        }

        ENTIX_API inline constexpr StringView GetName() const noexcept { return k_name; }
        ENTIX_API inline constexpr LogLevel GetMinLevel() const noexcept { return k_minLevel; }

        ENTIX_API inline constexpr bool IsEnabled(LogLevel level) const noexcept { return (Uint8)level >= (Uint8)k_minLevel; }

    private:
        const StringView k_name;
        const LogLevel k_minLevel;
    };
}