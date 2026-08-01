#include "Entix/RHI/Viewport.h"

namespace Entix::RHI
{
    Viewport::Viewport(float x_, float y_, float width_, float height_)
        : x(x_), y(y_), width(width_), height(height_)
    {
    }

    void Viewport::UpdateAbsolute(const Extent2D& targetSize) noexcept
    {
        m_absolutePos.x = static_cast<Uint32>(x * targetSize.x);
        m_absolutePos.y = static_cast<Uint32>(y * targetSize.y);
        m_absoluteSize.x = static_cast<Uint32>(width * targetSize.x);
        m_absoluteSize.y = static_cast<Uint32>(height * targetSize.y);
    }

    float Viewport::GetAspectRatio() const noexcept
    {
        return static_cast<float>(m_absoluteSize.x - m_absolutePos.x) / static_cast<float>(m_absoluteSize.y - m_absolutePos.y);
    }

    const Extent2D& Viewport::GetAbsolutePosition() const noexcept
    {
        return m_absolutePos;
    }

    const Extent2D& Viewport::GetAbsoluteSize() const noexcept
    {
        return m_absoluteSize;
    }
}