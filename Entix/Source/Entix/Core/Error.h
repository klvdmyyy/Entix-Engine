#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/String.h"

#include <source_location>

namespace Entix
{
    class Error
    {
    public:
        constexpr Error(StringView message, std::source_location location = std::source_location::current())
            : m_message(message), m_location(location)
        {
        }
        virtual ~Error() = default;

        ENTIX_API virtual String What() const noexcept;
        
        constexpr StringView GetMessage() const noexcept
        {
            return m_message;
        }

    protected:
        StringView m_message;
        std::source_location m_location;
    };
}