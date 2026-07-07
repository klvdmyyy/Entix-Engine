// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/Types.h"

class Timestep {
public:
    FORCE_INLINE
    inline constexpr Timestep(float time = 0.0f)
        : m_time(time)
    {
    }

    operator float() const { return m_time; }

private:
    float m_time;
};