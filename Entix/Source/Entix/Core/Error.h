#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/String.h"

#include <source_location>
#include <ranges>
#include <sstream>
#include <format>

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

template<>
struct std::formatter<Entix::Error, char>
{
    template<typename ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
        auto it = ctx.begin();
        return it;
    }

    template<typename FmtContext>
    FmtContext::iterator format(Entix::Error err, FmtContext& ctx) const
    {
        std::ostringstream out;
        out << err.What();

        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};