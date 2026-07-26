/**
 * @file
 * @brief Error base class.
 * 
 * @ingroup Core
 */

#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/String.h"

#include <source_location>
#include <ranges>
#include <sstream>
#include <format>

namespace Entix
{
    /**
     * @brief Error base class
     * 
     * @details
     * Mainly implemented for using in Result class. You can also
     * use it for throw/try/catch statements.
     * 
     * @ingroup Core
     */
    class Error
    {
    public:
        constexpr Error(StringView message, std::source_location location = std::source_location::current())
            : m_message(message), m_location(location)
        {
        }
        virtual ~Error() = default;

        /**
         * @brief Says what's wrong =)
         * 
         * @return String Formatter error message
         */
        ENTIX_API virtual String What() const noexcept;
        
        /**
         * @brief Just return raw error message
         * 
         * @return StringView
         */
        constexpr StringView GetMessage() const noexcept
        {
            return m_message;
        }

    protected:
        StringView m_message;
        std::source_location m_location;
    };
}

/**
 * @ingroup Core
 */
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