#pragma once

#include "Entix/Core/Types.h"

namespace Entix
{
    class Timestep
    {
    public:
        inline constexpr Timestep(float time = 0.0f)
            : m_time(time)
        {
        }

        inline constexpr operator float() const noexcept { return m_time; }

        inline constexpr float GetSeconds() const noexcept { return m_time; }
        inline constexpr float GetMilliseconds() const noexcept { return m_time * 1000.0f; }

    private:
        float m_time;
    };
}