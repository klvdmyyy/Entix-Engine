#pragma once

#include "Entix/Core/Base.h"
#include "Entix/RHI/Extent2D.h"

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

        ENTIX_API void UpdateAbsolute(const Extent2D& targetSize) noexcept;
        ENTIX_API float GetAspectRatio() const noexcept;
        ENTIX_API const Extent2D& GetAbsolutePosition() const noexcept;
        ENTIX_API const Extent2D& GetAbsoluteSize() const noexcept;

    private:
        Extent2D m_absolutePos{};
        Extent2D m_absoluteSize{};
    };
}