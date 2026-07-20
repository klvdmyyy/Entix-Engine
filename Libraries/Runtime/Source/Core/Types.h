// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"

#include "Core/String.h"

#include <cstddef>
#include <cstdint>

#include <format>
#include <sstream>

using Uint8 = uint8_t;
using Uint16 = uint16_t;
using Uint32 = uint32_t;
using Uint64 = uint64_t;
    
using Int8 = int8_t;
using Int16 = int16_t;
using Int32 = int32_t;
using Int64 = int64_t;


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

    String ToString() const noexcept {
        if(m_count >= 1024 * 1024 * 1024) {
            return std::format("{:.2f}GB", AsGigabytes());
        } else if(m_count >= 1024 * 1024) {
            return std::format("{:.2f}MB", AsMegabytes());
        } else if (m_count >= 1024) {
            return std::format("{:.2f}KB", AsKilobytes());
        } else {
            return std::format("{}B", AsBytes());
        }
    }

private:
    size_t m_count;
};

template<>
struct std::formatter<Bytes, char>
{
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
        auto it = ctx.begin();
        return it;
    }

    template<typename FmtContext>
    FmtContext::iterator format(Bytes bytes,
                                FmtContext &ctx) const
    {
        std::ostringstream out;
        out << bytes.ToString();

        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};

class Uuid {
public:
    Uuid();
    Uuid(Uint64 uuid);
    Uuid(const Uuid& other) = default;

    operator Uint64() const { return m_uuid; }

private:
    Uint64 m_uuid;
};

namespace std {
    template<typename T>
    struct hash;

    template<>
    struct hash<::Uuid>
    {
        std::size_t operator()(const ::Uuid& uuid) const {
            return (::Uint64)uuid;
        }
    };
}