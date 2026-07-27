#pragma once

#include "Entix/Core/Base.h"
#include "Entix/RHI/Rect.h"

namespace Entix::RHI
{
    class Viewport
    {
    public:
        ENTIX_API Viewport() = default;
        ENTIX_API Viewport(float x_, float y_, float width_, float height_);

        float x = 0.0f;
        float y = 0.0f;
        float width = 1.0f;
        float height = 1.0f;

        ENTIX_API void UpdateAbsolute(const Rect& targetSize) noexcept;
        ENTIX_API const float GetAspectRatio() const noexcept;
        ENTIX_API const Rect& GetAbsolutePosition() const noexcept;
        ENTIX_API const Rect& GetAbsoluteSize() const noexcept;

    private:
        Rect m_absolutePos{};
        Rect m_absoluteSize{};
    };
}