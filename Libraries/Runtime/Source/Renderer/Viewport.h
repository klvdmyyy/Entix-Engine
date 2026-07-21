#pragma once

#include "Core/Types.h"
#include "Core/Hash.h"

#include "Renderer/Rect.h"

namespace Renderer {
    class Viewport {
    public:
        float x = 0.0f;
        float y = 0.0f;
        float width = 1.0f;
        float height = 1.0f;

        Viewport() : x(0.0f), y(0.0f), width(1.0f), height(1.0f) {}
        Viewport(float x_, float y_, float w_, float h_)
            : x(x_), y(y_), width(w_), height(h_)
        {
        }

        void UpdateAbsolute(const Rect& targetSize) noexcept
        {
            absolutePos.x = static_cast<Uint32>(x * targetSize.x);
            absolutePos.y = static_cast<Uint32>(y * targetSize.y);
            absoluteSize.x = static_cast<Uint32>(width * targetSize.x);
            absoluteSize.y = static_cast<Uint32>(height * targetSize.y);
        }

        const float GetAspectRatio() const noexcept
        {
            return static_cast<float>(absoluteSize.x - absolutePos.x) / static_cast<float>(absoluteSize.y - absolutePos.y);
        }

        Rect GetAbsolutePosition() const noexcept
        {
            return absolutePos;
        }

        Rect GetAbsoluteSize() const noexcept
        {
            return absoluteSize;
        }

    private:
        Rect absolutePos{};
        Rect absoluteSize{};
    };
}

template<>
struct std::hash<Renderer::Viewport> {
    size_t operator()(const Renderer::Viewport& viewport) const {
        size_t seed = 0;
        HashCombine(seed, viewport.x, viewport.y, viewport.width, viewport.height);
        return seed;
    }
};