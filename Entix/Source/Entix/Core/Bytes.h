#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/String.h"

#include <cstddef>
#include <cstdint>

#include <format>
#include <sstream>
#include <ranges>

namespace Entix
{
    class Bytes {
    public:
        constexpr Bytes(size_t count) : m_count(count) {}

        constexpr size_t AsBytes() const noexcept { return m_count; }
        constexpr float AsKilobytes() const noexcept { return static_cast<float>(m_count) / 1024.0f; }
        constexpr float AsMegabytes() const noexcept { return AsKilobytes() / 1024.0f; }
        constexpr float AsGigabytes() const noexcept { return AsMegabytes() / 1024.0f; }

        constexpr Bytes operator+(const Bytes& other) {
            return Bytes(m_count + other.m_count);
        }

        constexpr void operator+=(const Bytes& other) {
            m_count += other.m_count;
        }

        constexpr Bytes operator-(const Bytes& other) {
            return Bytes(m_count - other.m_count);
        }

        constexpr void operator-=(const Bytes& other) {
            m_count -= other.m_count;
        }

        ENTIX_API String ToString() const noexcept;

    private:
        size_t m_count;
    };
}

template<>
struct std::formatter<::Entix::Bytes, char>
{
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
        auto it = ctx.begin();
        return it;
    }

    template<typename FmtContext>
    FmtContext::iterator format(::Entix::Bytes bytes,
                                FmtContext &ctx) const
    {
        std::ostringstream out;
        out << bytes.ToString();

        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};