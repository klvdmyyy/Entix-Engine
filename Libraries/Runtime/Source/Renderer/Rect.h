#pragma once

#include "Core/Types.h"

#include <format>

namespace Renderer {
    struct Rect {
        Uint32 x;
        Uint32 y;

        Rect() : x(0), y(0) {}
        Rect(Uint32 x_, Uint32 y_) : x(x_), y(y_) {}

        Rect operator*(const float& other)
        {
            return Rect(static_cast<Uint32>(x * other), static_cast<Uint32>(y * other));
        }

        Rect operator*(const Uint32& other)
        {
            return Rect(x * other, y * other);
        }

        Rect operator*(const Rect& other)
        {
            return Rect(x * other.x, y * other.y);
        }
    };
}

template<>
struct std::formatter<Renderer::Rect, char>
{
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
        auto it = ctx.begin();
        return it;
    }

    template<typename FmtContext>
    FmtContext::iterator format(Renderer::Rect rect,
                                FmtContext &ctx) const
    {
        std::ostringstream out;
        out << "{" << std::to_string(rect.x) << ", " << std::to_string(rect.y) << "}";

        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};
